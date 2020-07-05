#ifndef PARANOIA_PROCESS_EVENT_WRITER_H
#define PARANOIA_PROCESS_EVENT_WRITER_H

#include <iostream>
#include <sstream>

#include "msg_queue.h"
#include "process_event.h"
#include "procfs_cache.h"

enum class OutputFormat { TEXT, JSON };

class ProcessEventWriter {
 public:
  explicit ProcessEventWriter(ProcFSCache& cache, const OutputFormat& format = OutputFormat::TEXT, std::ostream& os = std::cout);
  [[noreturn]] void Write(MsgQueue<std::unique_ptr<ProcessEvent>>* queue);

 private:
  ProcFSCache& cache;
  std::unique_ptr<KVSerializer> serializer;
  std::ostream& os;
};

#endif  // PARANOIA_PROCESS_EVENT_WRITER_H
