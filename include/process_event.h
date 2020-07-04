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
  ProcessEvent(NetlinkMsg event, time_t timestamp) : event(event), timestamp(timestamp){};
  virtual void Annotate(ProcFSCache& cache) = 0;
  void Format(ProcFSCache& cache, const KVSerializer& serializer, std::ostream& os) const;

 protected:
  [[nodiscard]] virtual std::map<std::string, std::string> AsKeyValuePairs(ProcFSCache& cache) const = 0;
  virtual void PostWriteHook(ProcFSCache& cache) const {};
  NetlinkMsg event{};
  time_t timestamp{};
};

class NoneProcessEvent : public ProcessEvent {
 public:
  NoneProcessEvent(NetlinkMsg event, time_t timestamp) : ProcessEvent(event, timestamp){};
  void Annotate(ProcFSCache& cache) override;

 private:
  [[nodiscard]] std::map<std::string, std::string> AsKeyValuePairs(ProcFSCache& cache) const override;
};

class ForkProcessEvent : public ProcessEvent {
 public:
  ForkProcessEvent(NetlinkMsg event, time_t timestamp) : ProcessEvent(event, timestamp){};
  void Annotate(ProcFSCache& cache) override;

 private:
  [[nodiscard]] std::map<std::string, std::string> AsKeyValuePairs(ProcFSCache& cache) const override;
};

class ExecProcessEvent : public ProcessEvent {
 public:
  ExecProcessEvent(NetlinkMsg event, time_t timestamp) : ProcessEvent(event, timestamp){};
  void Annotate(ProcFSCache& cache) override;

 private:
  [[nodiscard]] std::map<std::string, std::string> AsKeyValuePairs(ProcFSCache& cache) const override;
};

class ExitProcessEvent : public ProcessEvent {
 public:
  ExitProcessEvent(NetlinkMsg event, time_t timestamp) : ProcessEvent(event, timestamp){};
  void Annotate(ProcFSCache& cache) override;

 private:
  [[nodiscard]] std::map<std::string, std::string> AsKeyValuePairs(ProcFSCache& cache) const override;
  void PostWriteHook(ProcFSCache& cache) const override;
};

#endif  // PARANOIA_PROCESS_EVENT_H
