#ifndef PARANOIA_SOCKET_H
#define PARANOIA_SOCKET_H

#include <linux/cn_proc.h>
#include <linux/connector.h>
#include <linux/netlink.h>
#include <sys/socket.h>
#include <unistd.h>
#include <util/concat.h>

#include <cerrno>
#include <cstring>
#include <stdexcept>

#include "netlink/error.h"

class Socket {
 public:
  Socket();
  ~Socket();

  Socket(const Socket& other) = delete;
  Socket& operator=(const Socket& other) = delete;

  Socket(Socket&& other) = delete;
  Socket& operator=(Socket&& other) = delete;

  void Bind() const;

  template <typename T>
  T Recv(int flags) const {
    T msg;
    size_t len = recv(fd, &msg, sizeof(msg), flags);
    if (len == -1) {
      throw ErrnoException(errno, "error receiving from socket");
    }
    return msg;
  }

  template <typename T>
  size_t Send(const T& msg, int flags) const {
    size_t len = send(fd, &msg, sizeof(msg), flags);
    if (len == -1) {
      throw ErrnoException(errno, "error sending from socket");
    }
    return len;
  }

 private:
  int fd;
};

#endif  // PARANOIA_SOCKET_H
