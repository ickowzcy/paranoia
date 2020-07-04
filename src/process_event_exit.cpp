#include "process_event_exit.h"

#include "process_event_keys.h"

void ExitProcessEvent::Annotate(ProcFSCache& cache) {}

std::map<std::string, std::string> ExitProcessEvent::AsKeyValuePairs(ProcFSCache& cache) const {
  std::map<std::string, std::string> kvs;

  pid_t pPid = ProcessEvent::event.proc_ev.event_data.exit.parent_pid;
  pid_t pTgid = ProcessEvent::event.proc_ev.event_data.exit.parent_tgid;
  kvs[PID_PARENT_KEY] = std::to_string(pPid);
  kvs[TGID_PARENT_KEY] = std::to_string(pTgid);
  kvs[CMDLINE_PARENT_KEY] = cache.Read(pTgid).cmdline.empty() ? cache.Read(pPid).cmdline : cache.Read(pTgid).cmdline;

  pid_t pid = ProcessEvent::event.proc_ev.event_data.exit.process_pid;
  pid_t tgid = ProcessEvent::event.proc_ev.event_data.exit.process_tgid;
  uint32_t exit_code = ProcessEvent::event.proc_ev.event_data.exit.exit_code;
  uint32_t exit_signal = ProcessEvent::event.proc_ev.event_data.exit.exit_signal;
  kvs[TYPE_KEY] = TYPE_EXIT;
  kvs[TIMESTAMP_KEY] = std::to_string(ProcessEvent::timestamp);
  kvs[PID_KEY] = std::to_string(pid);
  kvs[TGID_KEY] = std::to_string(tgid);
  kvs[EXIT_CODE_KEY] = std::to_string(exit_code);
  kvs[EXIT_SIGNAL_KEY] = std::to_string(exit_signal);

  kvs[CMDLINE_KEY] = cache.Read(tgid).cmdline.empty() ? cache.Read(pid).cmdline : cache.Read(tgid).cmdline;
  return std::move(kvs);
}

void ExitProcessEvent::PostWriteHook(ProcFSCache& cache) const {
  pid_t tgid = event.proc_ev.event_data.exit.process_tgid;
  cache.Invalidate(tgid);
}