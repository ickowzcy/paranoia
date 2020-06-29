#ifndef PARANOIA_NETLINKSOCKET_H
#define PARANOIA_NETLINKSOCKET_H

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

#include "KVSerializer.h"
#include "MessageQueue.h"
#include "ProcFSCache.h"
#include "map"

// Struct used to read events via the Netlink connector mechanism.
// See: https://github.com/torvalds/linux/blob/master/include/uapi/linux/cn_proc.h
struct __attribute__((aligned(NLMSG_ALIGNTO))) nlcn_msg {
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

  void Bind() const;
  void Subscribe() const;
  [[nodiscard]] nlcn_msg Recv() const;

 private:
  int nl_sock_{};
};

class interrupted_error : public std::exception {
 public:
  [[nodiscard]] const char* what() const noexcept override { return "EINTR: interrupted!"; }
};

#endif  // PARANOIA_NETLINKSOCKET_H
