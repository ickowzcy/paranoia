#include "netlink/connector.h"

#include <sys/errno.h>

#include "netlink/error.h"

Connector::Connector() {
  s.Bind();
  Subscribe();
}

void Connector::Subscribe() const {
  struct __attribute__((aligned(NLMSG_ALIGNTO))) {
    struct nlmsghdr nl_hdr;
    struct __attribute__((__packed__)) {
      struct cn_msg cn_msg;
      enum proc_cn_mcast_op cn_mcast;
    };
  } msg;

  memset(&msg, 0, sizeof(msg));
  msg.nl_hdr.nlmsg_len = sizeof(msg);
  msg.nl_hdr.nlmsg_pid = getpid();
  msg.nl_hdr.nlmsg_type = NLMSG_DONE;

  msg.cn_msg.id.idx = CN_IDX_PROC;
  msg.cn_msg.id.val = CN_VAL_PROC;
  msg.cn_msg.len = sizeof(enum proc_cn_mcast_op);

  msg.cn_mcast = PROC_CN_MCAST_LISTEN;

  size_t len = s.Send(msg, 0);
  if (len == -1) {
    throw ErrnoException(errno, "error subscribing to netlink connector");
  }
}

NetlinkMsg Connector::Recv() const { return s.Recv<NetlinkMsg>(0); }
