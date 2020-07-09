#ifndef PARANOIA_PE_KEYS_H
#define PARANOIA_PE_KEYS_H

constexpr auto TYPE_KEY = "type";
constexpr auto TYPE_NONE = "none";
constexpr auto TYPE_COREDUMP = "coredump";
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

#endif  // PARANOIA_PE_KEYS_H
