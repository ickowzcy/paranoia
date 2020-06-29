#ifndef PARANOIA_PROCESSEVENTWRITER_H
#define PARANOIA_PROCESSEVENTWRITER_H

#include <sstream>

#include "MessageQueue.h"
#include "ProcessEvent.h"
#include "ProcessEventListener.h"

enum class OutputFormat { TEXT, JSON };

class ProcessEventWriter {
 public:
  ProcessEventWriter(ProcFSCache& procFSCache, const OutputFormat& format, std::ostream& os);
  [[noreturn]] void write(MessageQueue<std::unique_ptr<ProcessEvent>>* queue);

 private:
  ProcFSCache& procFSCache_;
  std::unique_ptr<KVSerializer> serializer_;
  std::ostream& os_;
};

#endif  // PARANOIA_PROCESSEVENTWRITER_H
