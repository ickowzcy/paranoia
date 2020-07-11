#ifndef PARANOIA_KV_SERIALIZER_H
#define PARANOIA_KV_SERIALIZER_H

#include <map>

// Abstract class for key-value-pair serializers
class KVSerializer {
 public:
  [[nodiscard]] virtual std::string Serialize(const std::map<std::string, std::string>& kv_pairs) const = 0;
};

// Serializes key-value pairs into a human-readable text format
class KVTextSerializer : public KVSerializer {
 public:
  [[nodiscard]] std::string Serialize(const std::map<std::string, std::string>& kv_pairs) const override;
};

// Serializes key-value pairs into json
class KVJSONSerializer : public KVSerializer {
 public:
  [[nodiscard]] std::string Serialize(const std::map<std::string, std::string>& kv_pairs) const override;
};

#endif  // PARANOIA_KV_SERIALIZER_H
