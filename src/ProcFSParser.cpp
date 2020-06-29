#include "ProcFSParser.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string ProcFSParser::Command(int pid) {
  std::ifstream stream(PROC_DIRECTORY + std::to_string(pid) + CMDLINE_FILE);
  std::ostringstream oss;
  char c;
  while (stream >> std::noskipws >> c) {
    if (c == '\0') {
      oss << " ";
    } else {
      oss << c;
    }
  }
  auto s = oss.str();
  if (!s.empty()) {
    s.pop_back();  // remove last spurious whitespace due to null-char termination
  }
  return s;
}