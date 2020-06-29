#include "ProcessEventWriter.h"

#include <unordered_map>

#include "ProcFSCache.h"
#include "ProcessEvent.h"

ProcessEventWriter::ProcessEventWriter(ProcFSCache& procFSCache, const OutputFormat& format, std::ostream& os)
    : procFSCache_(procFSCache), os_(os) {
  if (OutputFormat::JSON == format) {
    serializer_ = std::make_unique<KVJSONSerializer>();
  }
  serializer_ = std::make_unique<KVTextSerializer>();
}

[[noreturn]] void ProcessEventWriter::write(MessageQueue<std::unique_ptr<ProcessEvent>>* queue) {
  while (true) {
    auto event = queue->receive();
    event->format(procFSCache_, *serializer_, os_);
  }
}