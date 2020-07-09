#ifndef PARANOIA_PE_EXIT_H
#define PARANOIA_PE_EXIT_H

#include <map>

#include "process_event.h"
#include "procfs_cache.h"

class ExitProcessEvent : public ProcessEvent {
 public:
  ExitProcessEvent(NetlinkMsg event, time_t timestamp) : ProcessEvent(event, timestamp){};
  void Annotate(ProcFSCache& cache) override;

 private:
  [[nodiscard]] std::map<std::string, std::string> AsKeyValuePairs(ProcFSCache& cache) const override;
  void PostWriteHook(ProcFSCache& cache) const override;
};

#endif  // PARANOIA_PE_EXIT_H
