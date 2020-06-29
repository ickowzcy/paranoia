#ifndef PARANOIA_PROCESSEVENT_H
#define PARANOIA_PROCESSEVENT_H

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

#include "KVSerializer.h"
#include "MessageQueue.h"
#include "NetlinkSocket.h"
#include "ProcFSCache.h"

class ProcessEvent {
 public:
  ProcessEvent(nlcn_msg event, time_t timestamp) : event(event), timestamp(timestamp){};
  virtual void annotate(ProcFSCache& procfsCache) = 0;
  void format(ProcFSCache& procfsCache, const KVSerializer& serializer, std::ostream& os) const;

 protected:
  [[nodiscard]] virtual std::map<std::string, std::string> asKeyValuePairs(ProcFSCache& procfsCache) const = 0;
  virtual void postWriteHook(ProcFSCache& procFsCache) const {};
  nlcn_msg event{};
  time_t timestamp{};
};

class NoneProcessEvent : public ProcessEvent {
 public:
  NoneProcessEvent(nlcn_msg event, time_t timestamp) : ProcessEvent(event, timestamp){};
  void annotate(ProcFSCache& procfsCache) override;

 private:
  [[nodiscard]] std::map<std::string, std::string> asKeyValuePairs(ProcFSCache& procfsCache) const;
};

class ForkProcessEvent : public ProcessEvent {
 public:
  ForkProcessEvent(nlcn_msg event, time_t timestamp) : ProcessEvent(event, timestamp){};
  void annotate(ProcFSCache& procfsCache) override;

 private:
  [[nodiscard]] std::map<std::string, std::string> asKeyValuePairs(ProcFSCache& procfsCache) const;
};

class ExecProcessEvent : public ProcessEvent {
 public:
  ExecProcessEvent(nlcn_msg event, time_t timestamp) : ProcessEvent(event, timestamp){};
  void annotate(ProcFSCache& procfsCache) override;

 private:
  [[nodiscard]] std::map<std::string, std::string> asKeyValuePairs(ProcFSCache& procfsCache) const;
};

class ExitProcessEvent : public ProcessEvent {
 public:
  ExitProcessEvent(nlcn_msg event, time_t timestamp) : ProcessEvent(event, timestamp){};
  void annotate(ProcFSCache& procfsCache) override;

 private:
  [[nodiscard]] std::map<std::string, std::string> asKeyValuePairs(ProcFSCache& procfsCache) const;
  void postWriteHook(ProcFSCache& procFsCache) const;
};

#endif  // PARANOIA_PROCESSEVENT_H
