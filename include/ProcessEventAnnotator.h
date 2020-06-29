#ifndef PARANOIA_PROCESSEVENTANNOTATOR_H
#define PARANOIA_PROCESSEVENTANNOTATOR_H

#include <string>
#include <unordered_map>

#include "MessageQueue.h"
#include "ProcessEventListener.h"
#include "ProcessEvent.h"

class ProcessEventAnnotator {
 public:
  ProcessEventAnnotator(ProcFSCache& procFSCache) : procFSCache_(procFSCache) {}

  [[noreturn]] void Annotate(MessageQueue<std::unique_ptr<ProcessEvent>>* from,
                             MessageQueue<std::unique_ptr<ProcessEvent>>* to);

 private:
  ProcFSCache& procFSCache_;
};

#endif  // PARANOIA_PROCESSEVENTANNOTATOR_H
