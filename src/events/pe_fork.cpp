#include "events/pe_fork.h"

#include "events/pe_keys.h"
#include "procfs_parser.h"

void ForkProcessEvent::UpdateCache(ProcFSCache& cache) const {
  pid_t pPid = ProcessEvent::event.proc_ev.event_data.fork.parent_pid;
  pid_t cPid = ProcessEvent::event.proc_ev.event_data.fork.child_pid;
  pid_t pTgid = ProcessEvent::event.proc_ev.event_data.fork.parent_tgid;
  pid_t cTgid = ProcessEvent::event.proc_ev.event_data.fork.child_tgid;
  cache.Refresh(pPid, ProcFSInfo{ProcFSParser::Cmd(pPid)});
  cache.Refresh(cPid, ProcFSInfo{ProcFSParser::Cmd(cPid)});
  cache.Refresh(pTgid, ProcFSInfo{ProcFSParser::Cmd(pTgid)});
  cache.Refresh(cTgid, ProcFSInfo{ProcFSParser::Cmd(cTgid)});
}

std::map<std::string, std::string> ForkProcessEvent::AsKeyValuePairs(ProcFSCache& cache) const {
  std::map<std::string, std::string> kvs;
  pid_t pPid = event.proc_ev.event_data.fork.parent_pid;
  pid_t cPid = event.proc_ev.event_data.fork.child_pid;
  pid_t pTgid = event.proc_ev.event_data.fork.parent_tgid;
  pid_t cTgid = event.proc_ev.event_data.fork.child_tgid;
  kvs[TYPE_KEY] = TYPE_FORK;
  kvs[TIMESTAMP_KEY] = std::to_string(timestamp);
  kvs[PID_PARENT_KEY] = std::to_string(pPid);
  kvs[PID_CHILD_KEY] = std::to_string(cPid);
  kvs[TGID_PARENT_KEY] = std::to_string(pTgid);
  kvs[TGID_CHILD_KEY] = std::to_string(cTgid);
  kvs[CMDLINE_PARENT_KEY] = cache.Read(pTgid).cmdline.empty() ? cache.Read(pPid).cmdline : cache.Read(pTgid).cmdline;
  kvs[CMDLINE_CHILD_KEY] = cache.Read(cTgid).cmdline.empty() ? cache.Read(cPid).cmdline : cache.Read(cTgid).cmdline;
  return kvs;
}