#include "util/time.h"

#include <iomanip>
#include <sstream>
#include <string>

std::string prettytimestamp(time_t t) {
  std::ostringstream oss;
  oss << std::put_time(std::localtime(&t), "%F %T %Z");
  return oss.str();
}
