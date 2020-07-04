#ifndef PARANOIA_KV_SERIALIZER_H
#define PARANOIA_KV_SERIALIZER_H

#include <map>

class KVSerializer {
 public:
  [[nodiscard]] virtual std::string Encode(const std::map<std::string, std::string>& keyValuePairs) const = 0;
};

class KVTextSerializer : public KVSerializer {
 public:
  [[nodiscard]] std::string Encode(const std::map<std::string, std::string>& keyValuePairs) const override;
};

class KVJSONSerializer : public KVSerializer {
 public:
  [[nodiscard]] std::string Encode(const std::map<std::string, std::string>& keyValuePairs) const override;
};

#endif  // PARANOIA_KV_SERIALIZER_H
