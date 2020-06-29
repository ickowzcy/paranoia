#ifndef PARANOIA_PROCFSCACHE_H
#define PARANOIA_PROCFSCACHE_H

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

#include <mutex>

#include "MessageQueue.h"
#include "unordered_map"

struct ProcFSInfo {
  std::string processName;
};

class ProcFSCache {
 public:
  void refresh(pid_t pid, ProcFSInfo data);
  ProcFSInfo read(pid_t);
  void invalidate(pid_t pid);

 private:
  std::unordered_map<pid_t, ProcFSInfo> procfsCache_;
  std::mutex mtx_;
};

#endif  // PARANOIA_PROCFSCACHE_H
