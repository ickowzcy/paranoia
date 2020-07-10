#ifndef PARANOIA_PE_EXEC_H
#define PARANOIA_PE_EXEC_H

#include <map>

#include "process_event.h"
#include "procfs_cache.h"

class ExecProcessEvent : public ProcessEvent {
 public:
  ExecProcessEvent(NetlinkMsg event, time_t timestamp) : ProcessEvent(event, timestamp){};

 private:
  void UpdateCache(ProcFSCache& cache) const override;
  [[nodiscard]] std::map<std::string, std::string> AsKeyValuePairs(ProcFSCache& cache) const override;
};

#endif  // PARANOIA_PE_EXEC_H
