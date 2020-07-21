#include "events/pe_exec.h"

#include "events/pe_keys.h"
#include "procfs_parser.h"

void ExecProcessEvent::UpdateCache(ProcFSCache& cache) const {
  pid_t pid = ProcessEvent::event.proc_ev.event_data.exec.process_pid;
  pid_t tgid = ProcessEvent::event.proc_ev.event_data.exec.process_tgid;
  cache.Refresh(pid, ProcFSInfo{ProcFSParser::Cmd(pid)});
  cache.Refresh(tgid, ProcFSInfo{ProcFSParser::Cmd(tgid)});
}

std::map<std::string, std::string> ExecProcessEvent::AsKeyValuePairs(ProcFSCache& cache) const {
  pid_t pid = event.proc_ev.event_data.exec.process_pid;
  pid_t tgid = event.proc_ev.event_data.exec.process_tgid;
  std::map<std::string, std::string> kvs;
  kvs[TYPE_KEY] = TYPE_EXEC;
  kvs[TIMESTAMP_KEY] = std::to_string(timestamp);
  kvs[PID_KEY] = std::to_string(pid);
  kvs[TGID_KEY] = std::to_string(tgid);
  kvs[CMDLINE_KEY] = cache.Read(tgid).cmdline.empty() ? cache.Read(pid).cmdline : cache.Read(tgid).cmdline;
  return kvs;
}