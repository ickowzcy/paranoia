#include "ProcessEvent.h"

#include <unistd.h>

#include "ProcFSCache.h"
#include "ProcFSParser.h"

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

void ProcessEvent::format(ProcFSCache& procfsCache, const KVSerializer& serializer, std::ostream& os) const {
  os << serializer.encode(asKeyValuePairs(procfsCache)) << "\n";
  postWriteHook(procfsCache);
}

std::map<std::string, std::string> NoneProcessEvent::asKeyValuePairs(ProcFSCache& _) const {
  std::map<std::string, std::string> kvs;
  kvs[TYPE_KEY] = TYPE_NONE;
  kvs[TIMESTAMP_KEY] = std::to_string(timestamp);
  return std::move(kvs);
}

void NoneProcessEvent::annotate(ProcFSCache& procfsCache) {}

void ForkProcessEvent::annotate(ProcFSCache& procfsCache) {
  pid_t pPid = event.proc_ev.event_data.fork.parent_pid;
  pid_t cPid = event.proc_ev.event_data.fork.child_pid;
  pid_t pTgid = event.proc_ev.event_data.fork.parent_tgid;
  pid_t cTgid = event.proc_ev.event_data.fork.child_tgid;
  procfsCache.refresh(pPid, std::move(ProcFSInfo{ProcFSParser::Command(pPid)}));
  procfsCache.refresh(cPid, std::move(ProcFSInfo{ProcFSParser::Command(cPid)}));
  procfsCache.refresh(pTgid, std::move(ProcFSInfo{ProcFSParser::Command(pTgid)}));
  procfsCache.refresh(cTgid, std::move(ProcFSInfo{ProcFSParser::Command(cTgid)}));
}

std::map<std::string, std::string> ForkProcessEvent::asKeyValuePairs(ProcFSCache& procfsCache) const {
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
  kvs[CMDLINE_PARENT_KEY] = procfsCache.read(pTgid).processName.empty() ? procfsCache.read(pPid).processName
                                                                        : procfsCache.read(pTgid).processName;
  kvs[CMDLINE_CHILD_KEY] = procfsCache.read(cTgid).processName.empty() ? procfsCache.read(cPid).processName
                                                                       : procfsCache.read(cTgid).processName;
  return std::move(kvs);
}

void ExecProcessEvent::annotate(ProcFSCache& procfsCache) {
  pid_t pid = event.proc_ev.event_data.exec.process_pid;
  pid_t tgid = event.proc_ev.event_data.exec.process_tgid;
  procfsCache.refresh(pid, std::move(ProcFSInfo{ProcFSParser::Command(pid)}));
  procfsCache.refresh(tgid, std::move(ProcFSInfo{ProcFSParser::Command(tgid)}));
}

std::map<std::string, std::string> ExecProcessEvent::asKeyValuePairs(ProcFSCache& procfsCache) const {
  pid_t pid = event.proc_ev.event_data.exec.process_pid;
  pid_t tgid = event.proc_ev.event_data.exec.process_tgid;
  std::map<std::string, std::string> kvs;
  kvs[TYPE_KEY] = TYPE_EXEC;
  kvs[TIMESTAMP_KEY] = std::to_string(timestamp);
  kvs[PID_KEY] = std::to_string(pid);
  kvs[TGID_KEY] = std::to_string(tgid);
  kvs[CMDLINE_KEY] = procfsCache.read(tgid).processName.empty() ? procfsCache.read(pid).processName
                                                                : procfsCache.read(tgid).processName;
  return std::move(kvs);
}

void ExitProcessEvent::annotate(ProcFSCache& procfsCache) {}

std::map<std::string, std::string> ExitProcessEvent::asKeyValuePairs(ProcFSCache& procfsCache) const {
  std::map<std::string, std::string> kvs;

  // TODO: not available in older kernels; add logic for detecting kernel release
  // pid_t pPid = event.proc_ev.event_data.exit.parent_pid;
  // pid_t pTgid = event.proc_ev.event_data.exit.parent_tgid;
  // kvs[PID_PARENT_KEY] = std::to_string(pPid);
  // kvs[TGID_PARENT_KEY] = std::to_string(pTgid);
  // kvs[CMDLINE_PARENT_KEY] = procfsCache.read(pTgid).processName.empty() ? procfsCache.read(pPid).processName
  //                                                                    : procfsCache.read(pTgid).processName;

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

  kvs[CMDLINE_KEY] = procfsCache.read(tgid).processName.empty() ? procfsCache.read(pid).processName
                                                                : procfsCache.read(tgid).processName;
  return std::move(kvs);
}

void ExitProcessEvent::postWriteHook(ProcFSCache& procfsCache) const {
  pid_t tgid = event.proc_ev.event_data.exit.process_tgid;
  procfsCache.invalidate(tgid);
}