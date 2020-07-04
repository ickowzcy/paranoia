#include "process_event_annotator.h"

#include "process_event.h"

void ProcessEventAnnotator::Annotate(MsgQueue<std::unique_ptr<ProcessEvent>> *from,
                                     MsgQueue<std::unique_ptr<ProcessEvent>> *to) {
  while (true) {
    auto msg = from->Receive();
      msg->Annotate(procfsCache);
      to->Send(std::move(msg));
  }
}
