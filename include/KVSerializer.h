#ifndef PARANOIA_KVSERIALIZER_H
#define PARANOIA_KVSERIALIZER_H

#include <map>

class KVSerializer {
 public:
  [[nodiscard]] virtual std::string encode(const std::map<std::string, std::string>& keyValuePairs) const = 0;
};

class KVTextSerializer : public KVSerializer {
 public:
  [[nodiscard]] std::string encode(const std::map<std::string, std::string>& keyValuePairs) const override;
};

class KVJSONSerializer : public KVSerializer {
 public:
  [[nodiscard]] std::string encode(const std::map<std::string, std::string>& keyValuePairs) const override;
};

#endif  // PARANOIA_KVSERIALIZER_H
