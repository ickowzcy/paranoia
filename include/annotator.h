#ifndef PARANOIA_ANNOTATOR_H
#define PARANOIA_ANNOTATOR_H

#include <string>
#include <unordered_map>

#include "msg_queue.h"
#include "listener.h"
#include "events/process_event.h"

class ProcessEventAnnotator {
 public:
  explicit ProcessEventAnnotator(ProcFSCache& procfsCache) : procfsCache(procfsCache) {}

  [[noreturn]] void Annotate(MsgQueue<std::unique_ptr<ProcessEvent>>* from,
                             MsgQueue<std::unique_ptr<ProcessEvent>>* to);

 private:
  ProcFSCache& procfsCache;
};

#endif  // PARANOIA_ANNOTATOR_H
