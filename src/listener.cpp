#include "listener.h"

#include <sys/socket.h>
#include <unistd.h>

#include <sstream>

#include "events/pe_coredump.h"
#include "events/pe_exec.h"
#include "events/pe_exit.h"
#include "events/pe_fork.h"
#include "events/pe_none.h"
#include "netlink_socket.h"
#include "procfs_cache.h"
#include "procfs_parser.h"
#include "util/log.h"

ProcessEventListener::ProcessEventListener() {
  nlsocket.Bind();
  nlsocket.Subscribe();
  RegisterEventFactories();
}

void ProcessEventListener::Listen(MsgQueue<std::unique_ptr<ProcessEvent>>* queue) const {
  auto clk = std::chrono::system_clock();

  NetlinkMsg event{};
  while (true) {
    try {
      event = nlsocket.Recv();
    } catch (InterruptedException& e) {
      int delaySecs = 2;
      log(e.what(), " (Retrying in ", delaySecs, " seconds)\n");
      std::this_thread::sleep_for(std::chrono::seconds(delaySecs));
      continue;
    }

    try {
      queue->Send(factories.at(event.proc_ev.what)(event, clk.to_time_t(clk.now())));
    } catch (std::out_of_range&) {
      // Ignore non registered events
    }
  }
}

void ProcessEventListener::RegisterEventFactories() {
  auto registerEventFactory = [this](EventType type, EventFactory&& factory) { factories[type] = std::move(factory); };

  registerEventFactory(proc_event::what::PROC_EVENT_NONE, MakeEventFactory<NoneProcessEvent>());
  registerEventFactory(proc_event::what::PROC_EVENT_FORK, MakeEventFactory<ForkProcessEvent>());
  registerEventFactory(proc_event::what::PROC_EVENT_EXEC, MakeEventFactory<ExecProcessEvent>());
  registerEventFactory(proc_event::what::PROC_EVENT_EXIT, MakeEventFactory<ExitProcessEvent>());
  registerEventFactory(proc_event::what::PROC_EVENT_COREDUMP, MakeEventFactory<CoredumpProcessEvent>());
}
