#ifndef PARANOIA_PROCESSEVENTLISTENER_H
#define PARANOIA_PROCESSEVENTLISTENER_H

#include <linux/cn_proc.h>
#include <linux/connector.h>
#include <linux/netlink.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_map>

#include "MessageQueue.h"
#include "NetlinkSocket.h"
#include "ProcFSCache.h"
#include "ProcessEvent.h"

using EventFactory = std::function<std::unique_ptr<ProcessEvent>(nlcn_msg, time_t)>;

// EventType refers to the "what" enum declared in
// https://github.com/torvalds/linux/blob/master/include/uapi/linux/cn_proc.h
using EventType = int;

class ProcessEventListener {
 public:
  ProcessEventListener();

  void Listen(MessageQueue<std::unique_ptr<ProcessEvent>>* queue) const;

 private:
  void RegisterEventFactories();

  NetlinkSocket nlSocket_;
  std::unordered_map<EventType, EventFactory> factories_;
};

#endif  // PARANOIA_PROCESSEVENTLISTENER_H