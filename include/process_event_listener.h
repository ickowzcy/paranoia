#ifndef PARANOIA_PROCESS_EVENT_LISTENER_H
#define PARANOIA_PROCESS_EVENT_LISTENER_H

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

#include "msg_queue.h"
#include "netlink_socket.h"
#include "process_event.h"
#include "procfs_cache.h"

using EventFactory = std::function<std::unique_ptr<ProcessEvent>(NetlinkMsg, time_t)>;

// EventType refers to the "what" enum declared in
// https://github.com/torvalds/linux/blob/master/include/uapi/linux/cn_proc.h
using EventType = int;

class ProcessEventListener {
 public:
  ProcessEventListener();

  [[noreturn]] void Listen(MsgQueue<std::unique_ptr<ProcessEvent>>* queue) const;

 private:
  void RegisterEventFactories();

  NetlinkSocket nlsocket;
  std::unordered_map<EventType, EventFactory> factories;
};

#endif  // PARANOIA_PROCESS_EVENT_LISTENER_H