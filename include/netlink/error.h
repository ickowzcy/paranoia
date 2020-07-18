#ifndef PARANOIA_ERROR_H
#define PARANOIA_ERROR_H

#include <sys/errno.h>

#include <cstring>
#include <stdexcept>

#include "util/concat.h"

using Errno = int;

struct ErrnoException : public std::runtime_error {
  template <typename... Args>
  ErrnoException(Errno err, Args... args) : std::runtime_error(concat(args...)) {
    this->err = err;
    this->msg = concat(std::runtime_error::what(), ": ", strerror(err));
  }

  const char* what() const noexcept override;

  Errno err;

 private:
  std::string msg;
};

#endif  // PARANOIA_ERROR_H
