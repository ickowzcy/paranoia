#ifndef PARANOIA_PROCESS_EVENT_EXEC_H
#define PARANOIA_PROCESS_EVENT_EXEC_H

#include <map>

#include "process_event.h"
#include "procfs_cache.h"

class ExecProcessEvent : public ProcessEvent {
 public:
  ExecProcessEvent(NetlinkMsg event, time_t timestamp) : ProcessEvent(event, timestamp){};
  void Annotate(ProcFSCache& cache) override;

 private:
  [[nodiscard]] std::map<std::string, std::string> AsKeyValuePairs(ProcFSCache& cache) const override;
};

#endif  // PARANOIA_PROCESS_EVENT_EXEC_H
