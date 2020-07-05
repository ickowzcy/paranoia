#include "kv_serializer.h"

#include "catch2/catch.hpp"
#include "process_event_keys.h"

TEST_CASE("Key/Value pairs are serialized to text", "[kv_serializer]") {
  using Catch::Matchers::EndsWith;

  KVTextSerializer serializer;
  std::map<std::string, std::string> kv_test{
      {TYPE_KEY, TYPE_EXEC}, {TIMESTAMP_KEY, "0"}, {"foo", "42"}, {"bar", "(- -)"}};

  REQUIRE_THAT(serializer.Encode(kv_test), EndsWith(" [ bar:\"(- -)\" foo:\"42\" ]"));
}

TEST_CASE("Key/Value pairs are serialized to json", "[kv_serializer]") {
  KVJSONSerializer serializer;
  std::map<std::string, std::string> kv_test{
      {TYPE_KEY, TYPE_EXEC}, {TIMESTAMP_KEY, "0"}, {"foo", "42"}, {"bar", "(- -)"}};

  REQUIRE("{\"bar\":\"(- -)\",\"foo\":\"42\",\"timestamp\":\"0\",\"type\":\"exec\"}" == serializer.Encode(kv_test));
}
