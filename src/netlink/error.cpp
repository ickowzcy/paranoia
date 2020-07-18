#include "netlink/error.h"

const char *ErrnoException::what() const noexcept { return msg.c_str(); }
