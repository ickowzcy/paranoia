#ifndef PARANOIA_PROCFS_CACHE_H
#define PARANOIA_PROCFS_CACHE_H

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

#include "msg_queue.h"
#include "unordered_map"

// ProcFSInfo represents information retrieved from the proc pseudo-filesystem.
struct ProcFSInfo {
  std::string cmdline;
};

// ProcFSCache is a cache for storing information retrieved from the proc
// pseudo-filesystem.
class ProcFSCache {
 public:
  void Refresh(pid_t pid, ProcFSInfo data);
  ProcFSInfo Read(pid_t pid);
  void Invalidate(pid_t pid);

 private:
  std::unordered_map<pid_t, ProcFSInfo> cache;
  std::mutex mtx;
};

#endif  // PARANOIA_PROCFS_CACHE_H
