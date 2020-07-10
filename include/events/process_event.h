#ifndef PARANOIA_PROCESS_EVENT_H
#define PARANOIA_PROCESS_EVENT_H

#include <errno.h>
#include <linux/cn_proc.h>
#include <linux/connector.h>
#include <linux/netlink.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <map>

#include "kv_serializer.h"
#include "msg_queue.h"
#include "netlink_socket.h"
#include "procfs_cache.h"

class ProcessEvent {
 public:
  virtual ~ProcessEvent() = default;

  void Annotate(ProcFSCache& cache) const;
  void Format(ProcFSCache& cache, const KVSerializer& serializer, std::ostream& os) const;

 protected:
  ProcessEvent(NetlinkMsg event, time_t timestamp) : event(event), timestamp(timestamp){};

  NetlinkMsg event{};
  time_t timestamp{};

 private:
  [[nodiscard]] virtual std::map<std::string, std::string> AsKeyValuePairs(ProcFSCache& cache) const = 0;
  virtual void UpdateCache(ProcFSCache& cache) const {};
  virtual void PostWriteHook(ProcFSCache& cache) const {};
};

#endif  // PARANOIA_PROCESS_EVENT_H
