#include "events/pe_coredump.h"

#include "events/pe_keys.h"

std::map<std::string, std::string> CoredumpProcessEvent::AsKeyValuePairs(ProcFSCache& cache) const {
  std::map<std::string, std::string> kvs;
  kvs.emplace(TYPE_KEY, TYPE_COREDUMP);
  kvs.emplace(TIMESTAMP_KEY, std::to_string(ProcessEvent::timestamp));

  pid_t pPid = ProcessEvent::event.proc_ev.event_data.coredump.parent_pid;
  pid_t pTgid = ProcessEvent::event.proc_ev.event_data.coredump.parent_tgid;
  kvs.emplace(PID_PARENT_KEY, std::to_string(pPid));
  kvs.emplace(TGID_PARENT_KEY, std::to_string(pTgid));
  kvs.emplace(CMDLINE_PARENT_KEY,
              cache.Read(pTgid).cmdline.empty() ? cache.Read(pPid).cmdline : cache.Read(pTgid).cmdline);

  pid_t pid = ProcessEvent::event.proc_ev.event_data.coredump.process_pid;
  pid_t tgid = ProcessEvent::event.proc_ev.event_data.coredump.process_tgid;
  kvs.emplace(PID_KEY, std::to_string(pid));
  kvs.emplace(TGID_KEY, std::to_string(tgid));
  kvs.emplace(CMDLINE_KEY, cache.Read(tgid).cmdline.empty() ? cache.Read(pid).cmdline : cache.Read(tgid).cmdline);

  return kvs;
}

void CoredumpProcessEvent::PostWriteHook(ProcFSCache& cache) const {
  pid_t tgid = event.proc_ev.event_data.coredump.process_tgid;
  cache.Invalidate(tgid);
}