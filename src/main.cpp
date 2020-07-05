#include <climits>
#include <cstring>
#include <iomanip>
#include <iostream>

#include "process_event.h"
#include "process_event_annotator.h"
#include "process_event_listener.h"
#include "process_event_writer.h"

OutputFormat select_format(const char* fmt) {
  if (fmt != nullptr && std::string(fmt) == "json") {
    return OutputFormat::JSON;
  }
  return OutputFormat::TEXT;
}

std::string hostname() {
  char h[HOST_NAME_MAX];
  int rc = gethostname(h, HOST_NAME_MAX);
  if (rc == -1) {
    std::cerr << "Error retrieving hostname: ";
    std::cerr << std::strerror(errno);
    std::cerr << "\n";
    return "[unknown-host]";
  }
  return std::string(h);
}

int main(int argc, const char* argv[]) {
  auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::cerr << "Paranoia started on " << hostname() << " at " << std::put_time(std::localtime(&now), "%F %T %Z")
            << "\n";

  MsgQueue<std::unique_ptr<ProcessEvent>> non_annotated_events;
  MsgQueue<std::unique_ptr<ProcessEvent>> annotated_events;
  ProcFSCache procfs_cache;

  ProcessEventWriter writer(procfs_cache, select_format(std::getenv("PARANOIA_OUTPUT_FORMAT")));

  ProcessEventAnnotator annotator(procfs_cache);

  ProcessEventListener listener;

  std::thread writer_thread(&ProcessEventWriter::Write, &writer, &annotated_events);
  std::thread annotator_thread(&ProcessEventAnnotator::Annotate, &annotator, &non_annotated_events, &annotated_events);

  std::cerr << "Spinning ← ↖ ↑ ↗ → ↘ ↓ ↙ ..."
            << "\n";

  // Next call blocks
  listener.Listen(&non_annotated_events);
}