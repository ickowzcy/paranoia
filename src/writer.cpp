#include "writer.h"

#include <unordered_map>

#include "events/process_event.h"
#include "procfs_cache.h"

ProcessEventWriter::ProcessEventWriter(ProcFSCache& cache, const OutputFormat& format, std::ostream& os)
    : cache(cache), os(os) {
  switch (format) {
    case OutputFormat::TEXT:
      serializer = std::make_unique<KVTextSerializer>();
      break;
    case OutputFormat::JSON:
      serializer = std::make_unique<KVJSONSerializer>();
      break;
    default:
      throw std::invalid_argument("unknown output format option");
  }
}

[[noreturn]] void ProcessEventWriter::Write(MsgQueue<std::unique_ptr<ProcessEvent>>* queue) {
  while (true) {
    auto event = queue->Receive();
    event->Format(cache, *serializer, os);
  }
}