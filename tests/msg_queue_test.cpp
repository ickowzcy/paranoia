#include "msg_queue.h"

#include <vector>

#include "catch2/catch.hpp"

TEST_CASE("Message queue is FIFO", "[msg_queue]") {
  using MsgType = int;
  std::vector<MsgType> msgs{10, 11, 23};
  MsgQueue<MsgType> queue;

  std::thread send_t([=, &queue]() {
    for (auto m : msgs) {
      queue.Send(std::move(m));
    }
  });

  std::thread receive_and_check_t([=, &queue, n = msgs.size()]() {
    for (int i = 0; i < n; i++) {
      REQUIRE(msgs[i] == queue.Receive());
    }
  });

  receive_and_check_t.join();
  send_t.join();
}
