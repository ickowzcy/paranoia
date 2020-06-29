#ifndef PARANOIA_QUEUE_H
#define PARANOIA_QUEUE_H

#include <algorithm>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

template <class T>
class MessageQueue {
 public:
  T receive() {
    std::unique_lock<std::mutex> uLock(mtx_);
    cond_.wait(uLock, [this] { return !messages_.empty(); });

    T msg = std::move(messages_.back());
    messages_.pop_back();

    return msg;
  }

  void send(T&& msg) {
    std::lock_guard<std::mutex> uLock(mtx_);

    messages_.push_back(std::move(msg));

    cond_.notify_one();
  }

 private:
  std::mutex mtx_;
  std::condition_variable cond_;
  std::deque<T> messages_;
};

#endif  // PARANOIA_QUEUE_H
