#include "listener.h"

#include <sys/socket.h>
#include <unistd.h>

#include <sstream>

#include "netlink_socket.h"
#include "events/pe_coredump.h"
#include "events/pe_exec.h"
#include "events/pe_exit.h"
#include "events/pe_fork.h"
#include "events/pe_none.h"
#include "procfs_cache.h"
#include "procfs_parser.h"

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
      std::cerr << e.what();
      std::cerr << " (Retrying in " << delaySecs << " seconds)";
      std::cerr << "\n";
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

  registerEventFactory(proc_event::what::PROC_EVENT_NONE,
                       [](NetlinkMsg event, time_t t) { return std::make_unique<NoneProcessEvent>(event, t); });

  registerEventFactory(proc_event::what::PROC_EVENT_FORK,
                       [](NetlinkMsg event, time_t t) { return std::make_unique<ForkProcessEvent>(event, t); });

  registerEventFactory(proc_event::what::PROC_EVENT_EXEC,
                       [](NetlinkMsg event, time_t t) { return std::make_unique<ExecProcessEvent>(event, t); });

  registerEventFactory(proc_event::what::PROC_EVENT_EXIT,
                       [](NetlinkMsg event, time_t t) { return std::make_unique<ExitProcessEvent>(event, t); });

  registerEventFactory(proc_event::what::PROC_EVENT_COREDUMP,
                       [](NetlinkMsg event, time_t t) { return std::make_unique<CoredumpProcessEvent>(event, t); });
}
