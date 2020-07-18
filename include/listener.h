#ifndef PARANOIA_LISTENER_H
#define PARANOIA_LISTENER_H

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

#include "events/process_event.h"
#include "msg_queue.h"
#include "netlink/connector.h"
#include "procfs_cache.h"

using EventFactory = std::function<std::unique_ptr<ProcessEvent>(NetlinkMsg, time_t)>;

// EventType refers to the "what" enum declared in
// https://github.com/torvalds/linux/blob/master/include/uapi/linux/cn_proc.h
using EventType = int;

// ProcessEventListener observes process-related events reported by the kernel.
// Each observed event is added to a MsgQueue.
class ProcessEventListener {
 public:
  ProcessEventListener();

  // Listens for events. Each observed event is added to the queue.
  [[noreturn]] void Listen(MsgQueue<std::unique_ptr<ProcessEvent>>* queue) const;

 private:
  // Registers an EventFactory for each known EventType.
  void RegisterEventFactories() noexcept;

  template <typename T>
  constexpr auto MakeEventFactory() {
    return [](NetlinkMsg event, time_t timestamp) { return std::make_unique<T>(event, timestamp); };
  }

  // NetlinkSocket nlsocket;
  Connector conn{};
  std::unordered_map<EventType, EventFactory> factories{};
};

#endif  // PARANOIA_LISTENER_H