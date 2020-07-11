#ifndef PARANOIA_QUEUE_H
#define PARANOIA_QUEUE_H

#include <algorithm>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

// MsgQueue represents an unbound message queue which can be used for
// communication between threads.
template <class T>
class MsgQueue {
 public:
  T Receive() {
    std::unique_lock<std::mutex> uLock(mtx);
    cond.wait(uLock, [this] { return !messages.empty(); });

    T msg = std::move(messages.back());
    messages.pop_back();

    return msg;
  }

  void Send(T&& msg) {
    std::lock_guard<std::mutex> uLock(mtx);

    messages.push_back(std::move(msg));

    cond.notify_one();
  }

 private:
  std::mutex mtx;
  std::condition_variable cond;
  std::deque<T> messages;
};

#endif  // PARANOIA_QUEUE_H
