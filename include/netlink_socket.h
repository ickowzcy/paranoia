#ifndef PARANOIA_NETLINK_SOCKET_H
#define PARANOIA_NETLINK_SOCKET_H

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
#include <exception>

#include "kv_serializer.h"
#include "msg_queue.h"
#include "procfs_cache.h"
#include "map"

// Struct used to read events via the Netlink connector mechanism.
// See: https://github.com/torvalds/linux/blob/master/include/uapi/linux/cn_proc.h
struct __attribute__((aligned(NLMSG_ALIGNTO))) NetlinkMsg {
  struct nlmsghdr nl_hdr;
  struct __attribute__((__packed__)) {
    struct cn_msg cn_msg;
    struct proc_event proc_ev;
  };
};

class NetlinkSocket {
 public:
  NetlinkSocket();
  ~NetlinkSocket();
  NetlinkSocket(NetlinkSocket& other) = delete;
  NetlinkSocket(NetlinkSocket&& other) = delete;
  NetlinkSocket& operator=(NetlinkSocket& other) = delete;
  NetlinkSocket& operator=(NetlinkSocket&& other) = delete;

  void Bind() const;
  void Subscribe() const;
  [[nodiscard]] NetlinkMsg Recv() const;

 private:
  int nl_sock{};
};

class InterruptedException : public std::exception {
 public:
  [[nodiscard]] const char* what() const noexcept override { return "EINTR: interrupted!"; }
};

#endif  // PARANOIA_NETLINK_SOCKET_H
