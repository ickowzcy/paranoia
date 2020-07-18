#include "netlink/socket.h"

#include <sys/socket.h>
#include <unistd.h>
#include <util/concat.h>

#include <cerrno>
#include <cstring>
#include <stdexcept>

Socket::Socket() {
  fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
  if (fd == -1) {
    throw ErrnoException(errno, "error creating socket");
  }
}

Socket::~Socket() { close(fd); }

void Socket::Bind() const {
  struct sockaddr_nl sa_nl;
  sa_nl.nl_family = AF_NETLINK;
  sa_nl.nl_groups = CN_IDX_PROC;
  sa_nl.nl_pid = getpid();

  int rc = bind(fd, (struct sockaddr*)&sa_nl, sizeof(sa_nl));
  if (rc == -1) {
    throw ErrnoException(errno, "error binding to netlink socket");
  }
}
