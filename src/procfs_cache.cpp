#include "procfs_cache.h"

void ProcFSCache::Refresh(pid_t pid, ProcFSInfo data) {
  std::lock_guard<std::mutex> lock(mtx);
    cache[pid] = std::move(data);
}

void ProcFSCache::Invalidate(pid_t pid) {
  std::lock_guard<std::mutex> lock(mtx);
  cache.erase(pid);
}

ProcFSInfo ProcFSCache::Read(pid_t pid) {
  std::lock_guard<std::mutex> lock(mtx);
  return cache[pid];
}
