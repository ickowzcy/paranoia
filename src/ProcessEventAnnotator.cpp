#include "ProcessEventAnnotator.h"

#include "ProcessEvent.h"

void ProcessEventAnnotator::Annotate(MessageQueue<std::unique_ptr<ProcessEvent>> *from,
                                     MessageQueue<std::unique_ptr<ProcessEvent>> *to) {
  while (true) {
    auto msg = from->receive();
    msg->annotate(procFSCache_);
    to->send(std::move(msg));
  }
}
