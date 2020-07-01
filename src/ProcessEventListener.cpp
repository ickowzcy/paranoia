#include "ProcessEventListener.h"

#include <sys/socket.h>
#include <unistd.h>

#include <sstream>

#include "NetlinkSocket.h"
#include "ProcFSCache.h"
#include "ProcFSParser.h"

ProcessEventListener::ProcessEventListener() {
  nlSocket_.Bind();
  nlSocket_.Subscribe();
  RegisterEventFactories();
}

void ProcessEventListener::Listen(MessageQueue<std::unique_ptr<ProcessEvent>>* queue) const {
  auto clk = std::chrono::system_clock();

  nlcn_msg event{};
  while (true) {
    try {
      event = nlSocket_.Recv();
    } catch (interrupted_error& e) {
      int delaySecs = 2;
      std::cerr << e.what();
      std::cerr << " (Retrying in " << delaySecs << " seconds)";
      std::cerr << "\n";
      std::this_thread::sleep_for(std::chrono::seconds(delaySecs));
      continue;
    }

    try {
      queue->send(factories_.at(event.proc_ev.what)(event, clk.to_time_t(clk.now())));
    } catch (std::out_of_range&) {
      // Ignore non registered events
    }
  }
}

void ProcessEventListener::RegisterEventFactories() {
  auto registerEventFactory = [this](EventType type, EventFactory&& factory) { factories_[type] = std::move(factory); };

  registerEventFactory(proc_event::what::PROC_EVENT_NONE,
                       [](nlcn_msg event, time_t t) { return std::make_unique<NoneProcessEvent>(event, t); });

  registerEventFactory(proc_event::what::PROC_EVENT_FORK,
                       [](nlcn_msg event, time_t t) { return std::make_unique<ForkProcessEvent>(event, t); });

  registerEventFactory(proc_event::what::PROC_EVENT_EXEC,
                       [](nlcn_msg event, time_t t) { return std::make_unique<ExecProcessEvent>(event, t); });

  registerEventFactory(proc_event::what::PROC_EVENT_EXIT,
                       [](nlcn_msg event, time_t t) { return std::make_unique<ExitProcessEvent>(event, t); });
}
