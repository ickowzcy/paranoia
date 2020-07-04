#include "process_event_writer.h"

#include <unordered_map>

#include "procfs_cache.h"
#include "process_event.h"

ProcessEventWriter::ProcessEventWriter(ProcFSCache& cache, const OutputFormat& format, std::ostream& os)
    : cache(cache), os(os) {
  if (OutputFormat::JSON == format) {
      serializer = std::make_unique<KVJSONSerializer>();
  }
    serializer = std::make_unique<KVTextSerializer>();
}

[[noreturn]] void ProcessEventWriter::Write(MsgQueue<std::unique_ptr<ProcessEvent>>* queue) {
  while (true) {
    auto event = queue->Receive();
      event->Format(cache, *serializer, os);
  }
}