#ifndef PARANOIA_KV_SERIALIZER_H
#define PARANOIA_KV_SERIALIZER_H

#include <map>

class KVSerializer {
 public:
  [[nodiscard]] virtual std::string Encode(const std::map<std::string, std::string>& kv_pairs) const = 0;
};

class KVTextSerializer : public KVSerializer {
 public:
  [[nodiscard]] std::string Encode(const std::map<std::string, std::string>& kv_pairs) const override;
};

class KVJSONSerializer : public KVSerializer {
 public:
  [[nodiscard]] std::string Encode(const std::map<std::string, std::string>& kv_pairs) const override;
};

#endif  // PARANOIA_KV_SERIALIZER_H
