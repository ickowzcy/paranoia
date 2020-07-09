#ifndef PARANOIA_PE_NONE_H
#define PARANOIA_PE_NONE_H

#include <map>

#include "process_event.h"
#include "procfs_cache.h"

class NoneProcessEvent : public ProcessEvent {
 public:
  NoneProcessEvent(NetlinkMsg event, time_t timestamp) : ProcessEvent(event, timestamp){};
  void Annotate(ProcFSCache& cache) override;

 private:
  [[nodiscard]] std::map<std::string, std::string> AsKeyValuePairs(ProcFSCache& cache) const override;
};

#include "process_event.h"

#endif  // PARANOIA_PE_NONE_H
