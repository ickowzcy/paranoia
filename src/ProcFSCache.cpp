#include "ProcFSCache.h"

void ProcFSCache::refresh(pid_t pid, ProcFSInfo data) {
  std::lock_guard<std::mutex> lock(mtx_);
  procfsCache_[pid] = std::move(data);
}

void ProcFSCache::invalidate(pid_t pid) {
  std::lock_guard<std::mutex> lock(mtx_);
  procfsCache_.erase(pid);
}

ProcFSInfo ProcFSCache::read(pid_t pid) {
  std::lock_guard<std::mutex> lock(mtx_);
  return procfsCache_[pid];
}
