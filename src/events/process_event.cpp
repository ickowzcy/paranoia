#include "events/process_event.h"

#include "procfs_cache.h"
#include "procfs_parser.h"

void ProcessEvent::Annotate(ProcFSCache& cache) const{
    UpdateCache(cache);
}

void ProcessEvent::Format(ProcFSCache& cache, const KVSerializer& serializer, std::ostream& os) const {
  os << serializer.Serialize(AsKeyValuePairs(cache)) << "\n";
  PostWriteHook(cache);
}
