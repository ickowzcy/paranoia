#ifndef PARANOIA_PROCESS_EVENT_ANNOTATOR_H
#define PARANOIA_PROCESS_EVENT_ANNOTATOR_H

#include <string>
#include <unordered_map>

#include "msg_queue.h"
#include "process_event_listener.h"
#include "process_event.h"

class ProcessEventAnnotator {
 public:
  explicit ProcessEventAnnotator(ProcFSCache& procfsCache) : procfsCache(procfsCache) {}

  [[noreturn]] void Annotate(MsgQueue<std::unique_ptr<ProcessEvent>>* from,
                             MsgQueue<std::unique_ptr<ProcessEvent>>* to);

 private:
  ProcFSCache& procfsCache;
};

#endif  // PARANOIA_PROCESS_EVENT_ANNOTATOR_H
