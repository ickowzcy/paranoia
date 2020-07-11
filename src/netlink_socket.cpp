#include "netlink_socket.h"

#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <sstream>

#include "util/concat.h"

NetlinkSocket::NetlinkSocket() {
  nlsock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
  if (nlsock == -1) {
    throw std::runtime_error(concat("Error creating socket: ", std::strerror(errno), "\n"));
  }
}

NetlinkSocket::~NetlinkSocket() { close(nlsock); }

void NetlinkSocket::Bind() const {
  struct sockaddr_nl sa_nl;
  sa_nl.nl_family = AF_NETLINK;
  sa_nl.nl_groups = CN_IDX_PROC;
  sa_nl.nl_pid = getpid();

  int rc = bind(nlsock, (struct sockaddr*)&sa_nl, sizeof(sa_nl));
  if (rc == -1) {
    close(nlsock);
    throw std::runtime_error(concat("Error binding to netlink socket: ", std::strerror(errno), "\n"));
  }
}

void NetlinkSocket::Subscribe() const {
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

  int rc = send(nlsock, &msg, sizeof(msg), 0);
  if (rc == -1) {
    throw std::runtime_error(concat("Error subscribing to netlink connector: ", std::strerror(errno), "\n"));
  }
}

NetlinkMsg NetlinkSocket::Recv() const {
  NetlinkMsg event;
  int rc = recv(nlsock, &event, sizeof(NetlinkMsg), 0);
  if (rc == -1) {
    if (errno == EINTR) {
      throw InterruptedException();
    }
    throw std::runtime_error(concat("Error receiving from netlink connector: ", std::strerror(errno), "\n"));
  }
  return event;
}
