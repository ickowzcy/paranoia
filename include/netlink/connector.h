#ifndef PARANOIA_CONNECTOR_H
#define PARANOIA_CONNECTOR_H

#include <linux/cn_proc.h>
#include <linux/connector.h>
#include <linux/netlink.h>

#include "netlink/error.h"
#include "netlink/socket.h"

// Struct used to read events via the Netlink connector mechanism.
// See: https://github.com/torvalds/linux/blob/master/include/uapi/linux/cn_proc.h
struct __attribute__((aligned(NLMSG_ALIGNTO))) NetlinkMsg {
  struct nlmsghdr nl_hdr;
  struct __attribute__((__packed__)) {
    struct cn_msg cn_msg;
    struct proc_event proc_ev;
  };
};

// A connector for receiving process events
class Connector {
 public:
  Connector();

  Connector(const Connector& other) = delete;
  Connector& operator=(const Connector& other) = delete;

  Connector(Connector&& other) = delete;
  Connector& operator=(Connector&& other) = delete;

  // Blocking call to receive an event.
  // May throw ErrnoException.
  [[nodiscard]] NetlinkMsg Recv() const;

 private:
  void Subscribe() const;

  Socket s{};
};

#endif  // PARANOIA_CONNECTOR_H
