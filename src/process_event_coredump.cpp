#include "process_event_coredump.h"

#include "process_event_keys.h"

void CoredumpProcessEvent::Annotate(ProcFSCache& cache) {}

std::map<std::string, std::string> CoredumpProcessEvent::AsKeyValuePairs(ProcFSCache& cache) const {
  std::map<std::string, std::string> kvs;
  kvs[TYPE_KEY] = TYPE_COREDUMP;
  kvs[TIMESTAMP_KEY] = std::to_string(ProcessEvent::timestamp);

  pid_t pPid = ProcessEvent::event.proc_ev.event_data.coredump.parent_pid;
  pid_t pTgid = ProcessEvent::event.proc_ev.event_data.coredump.parent_tgid;
  kvs[PID_PARENT_KEY] = std::to_string(pPid);
  kvs[TGID_PARENT_KEY] = std::to_string(pTgid);
  kvs[CMDLINE_PARENT_KEY] = cache.Read(pTgid).cmdline.empty() ? cache.Read(pPid).cmdline : cache.Read(pTgid).cmdline;

  pid_t pid = ProcessEvent::event.proc_ev.event_data.coredump.process_pid;
  pid_t tgid = ProcessEvent::event.proc_ev.event_data.coredump.process_tgid;
  kvs[PID_KEY] = std::to_string(pid);
  kvs[TGID_KEY] = std::to_string(tgid);
  kvs[CMDLINE_KEY] = cache.Read(tgid).cmdline.empty() ? cache.Read(pid).cmdline : cache.Read(tgid).cmdline;

  return std::move(kvs);
}

void CoredumpProcessEvent::PostWriteHook(ProcFSCache& cache) const {
  pid_t tgid = event.proc_ev.event_data.coredump.process_tgid;
  cache.Invalidate(tgid);
}