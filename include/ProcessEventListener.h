#ifndef PARANOIA_PROCESSEVENTLISTENER_H
#define PARANOIA_PROCESSEVENTLISTENER_H

#include <errno.h>
#include <linux/cn_proc.h>
#include <linux/connector.h>
#include <linux/netlink.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "MessageQueue.h"
#include "NetlinkSocket.h"
#include "ProcFSCache.h"
#include "ProcessEvent.h"

class ProcessEventListener {
 public:
  ProcessEventListener();

  void listen(MessageQueue<std::unique_ptr<ProcessEvent>>* queue) const;

 private:
  NetlinkSocket nlSocket;
};

#endif  // PARANOIA_PROCESSEVENTLISTENER_H