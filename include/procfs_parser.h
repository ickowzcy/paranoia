#ifndef PARANOIA_PROCFS_PARSER_H
#define PARANOIA_PROCFS_PARSER_H

#include <string>

// Collection of functions for accessing information from
// the proc pseudo-filesystem.
namespace ProcFSParser {
constexpr auto PROC_DIRECTORY = "/proc/";
constexpr auto CMDLINE_FILE = "/cmdline";

std::string Cmd(int pid);
}  // namespace ProcFSParser

#endif  // PARANOIA_PROCFS_PARSER_H
