#include "kv_serializer.h"

#include <iomanip>
#include <sstream>

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

  std::ostringstream expected;
  std::function<void(std::string, std::string, bool)> add_expected_term = [&expected](std::string&& k, std::string&& v,
                                                                                      bool add_sep) {
    expected << std::quoted(k);
    expected << ":";
    expected << std::quoted(v);
    if (add_sep) {
      expected << ",";
    }
  };

  expected << "{";
  add_expected_term("bar", "(- -)", true);
  add_expected_term("foo", "42", true);
  add_expected_term(TIMESTAMP_KEY, "0", true);
  add_expected_term(TYPE_KEY, TYPE_EXEC, false);
  expected << "}";

  REQUIRE(expected.str() == serializer.Encode(kv_test));
}
