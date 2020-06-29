#include "NetlinkSocket.h"

#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <sstream>

NetlinkSocket::NetlinkSocket() {
  nl_sock_ = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
  if (nl_sock_ == -1) {
    std::ostringstream oss;
    oss << "Error creating socket: ";
    oss << std::strerror(errno);
    oss << "\n";
    throw std::runtime_error(oss.str());
  }
}

NetlinkSocket::~NetlinkSocket() { close(nl_sock_); }

void NetlinkSocket::Bind() const {
  struct sockaddr_nl sa_nl;
  sa_nl.nl_family = AF_NETLINK;
  sa_nl.nl_groups = CN_IDX_PROC;
  sa_nl.nl_pid = getpid();

  int rc = bind(nl_sock_, (struct sockaddr*)&sa_nl, sizeof(sa_nl));
  if (rc == -1) {
    std::ostringstream oss;
    oss << "Error binding to netlink socket: ";
    oss << std::strerror(errno);
    oss << "\n";
    close(nl_sock_);
    throw std::runtime_error(oss.str());
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

  int rc = send(nl_sock_, &msg, sizeof(msg), 0);
  if (rc == -1) {
    std::ostringstream oss;
    oss << "Error subscribing to netlink connector: ";
    oss << std::strerror(errno);
    oss << "\n";
    throw std::runtime_error(oss.str());
  }
}

nlcn_msg NetlinkSocket::Recv() const {
  nlcn_msg event;
  int rc = recv(nl_sock_, &event, sizeof(nlcn_msg), 0);
  if (rc == -1) {
    if (errno == EINTR) {
        throw interrupted_error();
    }
    std::ostringstream oss;
    oss << "Error receiving from netlink connector: ";
    oss << std::strerror(errno);
    oss << "\n";
    throw std::runtime_error(oss.str());
  }
  return event;
}
