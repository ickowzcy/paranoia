#ifndef PARANOIA_PROCESS_EVENT_COREDUMP_H
#define PARANOIA_PROCESS_EVENT_COREDUMP_H

#include <map>

#include "process_event.h"
#include "procfs_cache.h"

class CoredumpProcessEvent : public ProcessEvent {
 public:
  CoredumpProcessEvent(NetlinkMsg event, time_t timestamp) : ProcessEvent(event, timestamp){};
  void Annotate(ProcFSCache& cache) override;

 private:
  [[nodiscard]] std::map<std::string, std::string> AsKeyValuePairs(ProcFSCache& cache) const override;
  void PostWriteHook(ProcFSCache& cache) const override;
};

#endif  // PARANOIA_PROCESS_EVENT_COREDUMP_H
