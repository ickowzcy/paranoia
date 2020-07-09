#include "kv_serializer.h"

#include <algorithm>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <sstream>

#include "events/pe_keys.h"

std::string toupper(std::string s) {
  std::locale l;
  std::transform(s.begin(), s.end(), s.begin(), [l](auto& c) { return std::toupper(c, l); });
  return s;
}

std::string prettytimestamp(time_t t) {
  std::ostringstream oss;
  oss << std::put_time(std::localtime(&t), "%F %T %Z");
  return oss.str();
};

std::string KVTextSerializer::Encode(const std::map<std::string, std::string>& kv_pairs) const {
  std::ostringstream oss;
  oss << prettytimestamp(std::stoi(kv_pairs.at(TIMESTAMP_KEY)));
  oss << " : ";
  oss << toupper(kv_pairs.at(TYPE_KEY));
  oss << " → ";
  oss << "[ ";

  for (const auto& kv : kv_pairs) {
    if (TYPE_KEY == kv.first || TIMESTAMP_KEY == kv.first) {
      continue;
    }
    oss << kv.first;
    oss << ":";
    oss << std::quoted(kv.second);
    oss << " ";
  }

  oss << "]";

  return oss.str();
}

std::string KVJSONSerializer::Encode(const std::map<std::string, std::string>& kv_pairs) const {
  nlohmann::json j(kv_pairs);
  std::ostringstream oss;
  oss << j;
  return oss.str();
}
