#include "process_event.h"

#include <unistd.h>

#include "procfs_cache.h"
#include "procfs_parser.h"

constexpr auto TYPE_KEY = "type";
constexpr auto TYPE_NONE = "none";
constexpr auto TYPE_FORK = "fork";
constexpr auto TYPE_EXEC = "exec";
constexpr auto TYPE_EXIT = "exit";
constexpr auto TIMESTAMP_KEY = "timestamp";
constexpr auto PID_KEY = "pid";
constexpr auto PID_PARENT_KEY = "parent_pid";
constexpr auto PID_CHILD_KEY = "child_pid";
constexpr auto TGID_KEY = "tgid";
constexpr auto TGID_PARENT_KEY = "parent_tgid";
constexpr auto TGID_CHILD_KEY = "child_tgid";
constexpr auto CMDLINE_KEY = "cmd";
constexpr auto CMDLINE_PARENT_KEY = "parent_cmd";
constexpr auto CMDLINE_CHILD_KEY = "child_cmd";
constexpr auto EXIT_CODE_KEY = "exit_code";
constexpr auto EXIT_SIGNAL_KEY = "exit_signal";

void ProcessEvent::Format(ProcFSCache& cache, const KVSerializer& serializer, std::ostream& os) const {
  os << serializer.Encode(AsKeyValuePairs(cache)) << "\n";
    PostWriteHook(cache);
}

std::map<std::string, std::string> NoneProcessEvent::AsKeyValuePairs(ProcFSCache& cache) const {
  std::map<std::string, std::string> kvs;
  kvs[TYPE_KEY] = TYPE_NONE;
  kvs[TIMESTAMP_KEY] = std::to_string(timestamp);
  return std::move(kvs);
}

void NoneProcessEvent::Annotate(ProcFSCache& cache) {}

void ForkProcessEvent::Annotate(ProcFSCache& cache) {
  pid_t pPid = event.proc_ev.event_data.fork.parent_pid;
  pid_t cPid = event.proc_ev.event_data.fork.child_pid;
  pid_t pTgid = event.proc_ev.event_data.fork.parent_tgid;
  pid_t cTgid = event.proc_ev.event_data.fork.child_tgid;
    cache.Refresh(pPid, std::move(ProcFSInfo{ProcFSParser::Command(pPid)}));
    cache.Refresh(cPid, std::move(ProcFSInfo{ProcFSParser::Command(cPid)}));
    cache.Refresh(pTgid, std::move(ProcFSInfo{ProcFSParser::Command(pTgid)}));
    cache.Refresh(cTgid, std::move(ProcFSInfo{ProcFSParser::Command(cTgid)}));
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
  kvs[CMDLINE_PARENT_KEY] = cache.Read(pTgid).cmdline.empty() ? cache.Read(pPid).cmdline
                                                              : cache.Read(pTgid).cmdline;
  kvs[CMDLINE_CHILD_KEY] = cache.Read(cTgid).cmdline.empty() ? cache.Read(cPid).cmdline
                                                             : cache.Read(cTgid).cmdline;
  return std::move(kvs);
}

void ExecProcessEvent::Annotate(ProcFSCache& cache) {
  pid_t pid = event.proc_ev.event_data.exec.process_pid;
  pid_t tgid = event.proc_ev.event_data.exec.process_tgid;
    cache.Refresh(pid, std::move(ProcFSInfo{ProcFSParser::Command(pid)}));
    cache.Refresh(tgid, std::move(ProcFSInfo{ProcFSParser::Command(tgid)}));
}

std::map<std::string, std::string> ExecProcessEvent::AsKeyValuePairs(ProcFSCache& cache) const {
  pid_t pid = event.proc_ev.event_data.exec.process_pid;
  pid_t tgid = event.proc_ev.event_data.exec.process_tgid;
  std::map<std::string, std::string> kvs;
  kvs[TYPE_KEY] = TYPE_EXEC;
  kvs[TIMESTAMP_KEY] = std::to_string(timestamp);
  kvs[PID_KEY] = std::to_string(pid);
  kvs[TGID_KEY] = std::to_string(tgid);
  kvs[CMDLINE_KEY] = cache.Read(tgid).cmdline.empty() ? cache.Read(pid).cmdline
                                                      : cache.Read(tgid).cmdline;
  return std::move(kvs);
}

void ExitProcessEvent::Annotate(ProcFSCache& cache) {}

std::map<std::string, std::string> ExitProcessEvent::AsKeyValuePairs(ProcFSCache& cache) const {
  std::map<std::string, std::string> kvs;

  pid_t pPid = event.proc_ev.event_data.exit.parent_pid;
  pid_t pTgid = event.proc_ev.event_data.exit.parent_tgid;
  kvs[PID_PARENT_KEY] = std::to_string(pPid);
  kvs[TGID_PARENT_KEY] = std::to_string(pTgid);
  kvs[CMDLINE_PARENT_KEY] = cache.Read(pTgid).cmdline.empty() ? cache.Read(pPid).cmdline
                                                              : cache.Read(pTgid).cmdline;

  pid_t pid = event.proc_ev.event_data.exit.process_pid;
  pid_t tgid = event.proc_ev.event_data.exit.process_tgid;
  uint32_t exit_code = event.proc_ev.event_data.exit.exit_code;
  uint32_t exit_signal = event.proc_ev.event_data.exit.exit_signal;
  kvs[TYPE_KEY] = TYPE_EXIT;
  kvs[TIMESTAMP_KEY] = std::to_string(timestamp);
  kvs[PID_KEY] = std::to_string(pid);
  kvs[TGID_KEY] = std::to_string(tgid);
  kvs[EXIT_CODE_KEY] = std::to_string(exit_code);
  kvs[EXIT_SIGNAL_KEY] = std::to_string(exit_signal);

  kvs[CMDLINE_KEY] = cache.Read(tgid).cmdline.empty() ? cache.Read(pid).cmdline
                                                      : cache.Read(tgid).cmdline;
  return std::move(kvs);
}

void ExitProcessEvent::PostWriteHook(ProcFSCache& cache) const {
  pid_t tgid = event.proc_ev.event_data.exit.process_tgid;
    cache.Invalidate(tgid);
}