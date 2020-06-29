#include <climits>
#include <cstring>
#include <iomanip>
#include <iostream>

#include "ProcessEvent.h"
#include "ProcessEventAnnotator.h"
#include "ProcessEventListener.h"
#include "ProcessEventWriter.h"

OutputFormat selectFormat(const char* format) {
  if (format != nullptr && std::string(format) == "json") {
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
  std::cerr << "Paranoia started on " << hostname() << " at " << std::put_time(std::localtime(&now), "%F %T %Z") << "\n";

  MessageQueue<std::unique_ptr<ProcessEvent>> nonAnnotatedEvents;
  MessageQueue<std::unique_ptr<ProcessEvent>> annotatedEvents;
  ProcFSCache procFSCache;

  ProcessEventWriter writer(procFSCache, selectFormat(std::getenv("PARANOIA_OUT_FORMAT")), std::cout);

  ProcessEventAnnotator annotator(procFSCache);

  ProcessEventListener listener;

  std::thread writerThread(&ProcessEventWriter::write, &writer, &annotatedEvents);
  std::thread annotatorThread(&ProcessEventAnnotator::Annotate, &annotator, &nonAnnotatedEvents, &annotatedEvents);

  std::cerr << "Spinning ← ↖ ↑ ↗ → ↘ ↓ ↙ ..."
            << "\n";

  listener.listen(&nonAnnotatedEvents);

  annotatorThread.join();
  writerThread.join();
}