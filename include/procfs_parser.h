#ifndef PARANOIA_PROCFS_PARSER_H
#define PARANOIA_PROCFS_PARSER_H

#include <string>

namespace ProcFSParser {
constexpr auto PROC_DIRECTORY = "/proc/";
constexpr auto CMDLINE_FILE = "/cmdline";

std::string Command(int pid);
}  // namespace ProcFSParser

#endif  // PARANOIA_PROCFS_PARSER_H
