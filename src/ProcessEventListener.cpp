#include "ProcessEventListener.h"

#include <sys/socket.h>
#include <unistd.h>

#include <sstream>

#include "ProcFSCache.h"
#include "ProcFSParser.h"
#include "NetlinkSocket.h"

ProcessEventListener::ProcessEventListener() {
  nlSocket.Bind();
  nlSocket.Subscribe();
}

void ProcessEventListener::listen(MessageQueue<std::unique_ptr<ProcessEvent>>* queue) const {
  auto clk = std::chrono::system_clock();

  nlcn_msg event{};
  while (true) {
    try {
      event = nlSocket.Recv();
    } catch (interrupted_error& e) {
      int delaySecs = 2;
      std::cerr << e.what();
      std::cerr << " (Retrying in " << delaySecs << " seconds)";
      std::cerr << "\n";
      std::this_thread::sleep_for(std::chrono::seconds(delaySecs));
      continue;
    }

    std::unique_ptr<ProcessEvent> msg;
    switch (event.proc_ev.what) {
      case proc_event::what::PROC_EVENT_NONE:
        msg = std::make_unique<NoneProcessEvent>(event, clk.to_time_t(clk.now()));
        break;
      case proc_event::what::PROC_EVENT_FORK:
        msg = std::make_unique<ForkProcessEvent>(event, clk.to_time_t(clk.now()));
        break;
      case proc_event::what::PROC_EVENT_EXEC:
        msg = std::make_unique<ExecProcessEvent>(event, clk.to_time_t(clk.now()));
        break;
      case proc_event::what::PROC_EVENT_EXIT:
        msg = std::make_unique<ExitProcessEvent>(event, clk.to_time_t(clk.now()));
        break;
      default:
        continue;
    }
    queue->send(std::move(msg));
  }
}