#include "KVSerializer.h"

#include <nlohmann/json.hpp>
#include <sstream>

std::string KVTextSerializer::encode(const std::map<std::string, std::string>& keyValuePairs) const {
  std::ostringstream oss;
  oss << "[ ";

  for (const auto& kv : keyValuePairs) {
    oss << kv.first;
    oss << ":";
    oss << "\"" << kv.second << "\"";
    oss << " ";
  }

  oss << "]";

  return oss.str();
}

std::string KVJSONSerializer::encode(const std::map<std::string, std::string>& keyValuePairs) const {
  nlohmann::json j(keyValuePairs);
  std::ostringstream oss;
  oss << j;
  return oss.str();
}
