#include "events/pe_exit.h"

#include "events/pe_keys.h"

std::map<std::string, std::string> ExitProcessEvent::AsKeyValuePairs(ProcFSCache& cache) const {
  std::map<std::string, std::string> kvs;
  kvs.emplace(TYPE_KEY, TYPE_EXIT);
  kvs.emplace(TIMESTAMP_KEY, std::to_string(ProcessEvent::timestamp));

  pid_t pPid = ProcessEvent::event.proc_ev.event_data.exit.parent_pid;
  pid_t pTgid = ProcessEvent::event.proc_ev.event_data.exit.parent_tgid;
  kvs.emplace(PID_PARENT_KEY, std::to_string(pPid));
  kvs.emplace(TGID_PARENT_KEY, std::to_string(pTgid));
  kvs.emplace(CMDLINE_PARENT_KEY,
              cache.Read(pTgid).cmdline.empty() ? cache.Read(pPid).cmdline : cache.Read(pTgid).cmdline);

  pid_t pid = ProcessEvent::event.proc_ev.event_data.exit.process_pid;
  pid_t tgid = ProcessEvent::event.proc_ev.event_data.exit.process_tgid;
  uint32_t exit_code = ProcessEvent::event.proc_ev.event_data.exit.exit_code;
  uint32_t exit_signal = ProcessEvent::event.proc_ev.event_data.exit.exit_signal;
  kvs.emplace(PID_KEY, std::to_string(pid));
  kvs.emplace(TGID_KEY, std::to_string(tgid));
  kvs.emplace(CMDLINE_KEY, cache.Read(tgid).cmdline.empty() ? cache.Read(pid).cmdline : cache.Read(tgid).cmdline);

  kvs.emplace(EXIT_CODE_KEY, std::to_string(exit_code));
  kvs.emplace(EXIT_SIGNAL_KEY, std::to_string(exit_signal));

  return kvs;
}

void ExitProcessEvent::PostWriteHook(ProcFSCache& cache) const {
  pid_t tgid = event.proc_ev.event_data.exit.process_tgid;
  cache.Invalidate(tgid);
}