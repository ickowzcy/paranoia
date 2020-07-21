#include "events/pe_none.h"

#include "events/pe_keys.h"

std::map<std::string, std::string> NoneProcessEvent::AsKeyValuePairs(ProcFSCache& cache) const {
  std::map<std::string, std::string> kvs;
  kvs.emplace(TYPE_KEY, TYPE_NONE);
  kvs.emplace(TIMESTAMP_KEY, std::to_string(ProcessEvent::timestamp));
  return kvs;
}
