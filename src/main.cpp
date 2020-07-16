#include <climits>
#include <cstring>
#include <iomanip>
#include <iostream>

#include "annotator.h"
#include "events/process_event.h"
#include "listener.h"
#include "util/log.h"
#include "util/time.h"
#include "writer.h"

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
    log("Error retrieving hostname: ", std::strerror(errno), "\n");
    return "[unknown-host]";
  }
  return std::string(h);
}

int main(int argc, const char* argv[]) {
  auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  log("Paranoia started on ", hostname(), " at ", prettytimestamp(now), "\n");

  MsgQueue<std::unique_ptr<ProcessEvent>> non_annotated_events;
  MsgQueue<std::unique_ptr<ProcessEvent>> annotated_events;
  ProcFSCache procfs_cache;

  ProcessEventWriter writer(procfs_cache, select_format(std::getenv("PARANOIA_OUTPUT_FORMAT")));

  ProcessEventAnnotator annotator(procfs_cache);

  std::thread writer_thread(&ProcessEventWriter::Write, &writer, &annotated_events);
  std::thread annotator_thread(&ProcessEventAnnotator::Annotate, &annotator, &non_annotated_events, &annotated_events);

  try {
    ProcessEventListener listener;

    // Next call blocks
    listener.Listen(&non_annotated_events);
  } catch (std::runtime_error& e) {
    log("Exiting due to runtime error: ", e.what());
    std::exit(1);
  }
}