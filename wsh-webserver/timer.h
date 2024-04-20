#pragma once

#include <iostream>
#include <thread>
#include <chrono>

class ka_timer {
public:
    ka_timer(int millseconds) : millseconds(millseconds), m_expired(false) {}
    void start() {
        m_thread = std::thread(&ka_timer::run, this);
        m_thread.detach();
    }
    void terminate() {
        m_expired = true;
    }
    bool expired() const {
        return m_expired;
    }
    void join() {
        m_thread.join();
    }
private:
    void run() {
        std::this_thread::sleep_for(std::chrono::milliseconds(millseconds));
        m_expired = true;
    }
    int millseconds;
    std::thread m_thread;
    bool m_expired;
};
