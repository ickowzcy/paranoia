#ifndef PARANOIA_PE_COREDUMP_H
#define PARANOIA_PE_COREDUMP_H

#include <map>

#include "process_event.h"
#include "procfs_cache.h"

class CoredumpProcessEvent : public ProcessEvent {
 public:
  CoredumpProcessEvent(NetlinkMsg event, time_t timestamp) : ProcessEvent(event, timestamp){};

 private:
  [[nodiscard]] std::map<std::string, std::string> AsKeyValuePairs(ProcFSCache& cache) const override;
  void PostWriteHook(ProcFSCache& cache) const override;
};

#endif  // PARANOIA_PE_COREDUMP_H
