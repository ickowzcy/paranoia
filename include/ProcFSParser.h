#ifndef PARANOIA_PROCFSPARSER_H
#define PARANOIA_PROCFSPARSER_H

#include <string>

namespace ProcFSParser {
constexpr auto PROC_DIRECTORY = "/proc/";
constexpr auto CMDLINE_FILE = "/cmdline";

std::string Command(int pid);
}  // namespace ProcFSParser

#endif  // PARANOIA_PROCFSPARSER_H
