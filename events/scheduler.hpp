#pragma once

#include <list>
#include <thread>
#include <functional>
#include <mutex>
#include <vector>
#include <atomic>

namespace NSolana {

    template <typename TEventType>
    class TSimpleScheduler {
    private:
        const size_t msInterval_;
        std::thread mainThread_;
        std::list<TEventType> events_;
        std::mutex eventsListMutex_;
        std::vector<std::thread> eventsThreads_;
        std::atomic<bool> needsToStop_;
    public:
        TSimpleScheduler(size_t msInterval)
            : msInterval_(msInterval), needsToStop_(false)
        {}

        virtual ~TSimpleScheduler() = default;
        
        void start() {
            mainThread_ = std::thread(
                [&]() {
                    while (!needsToStop_.load()) {
                        {
                            std::lock_guard<std::mutex> lg(eventsListMutex_);
                            if (!events_.empty()) {
                                eventsThreads_.emplace_back(events_.front());
                                events_.pop_front();
                            }
                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(msInterval_));
                    }
                    //  If some events remain in list - handle it
                    while (!events_.empty()) {
                        eventsThreads_.emplace_back(events_.front());
                        events_.pop_front();
                        std::this_thread::sleep_for(std::chrono::milliseconds(msInterval_));
                    }
                }
            );
        }

        void stop() {
            needsToStop_.store(true);
        }

        bool addEvent(TEventType newEvent) {
            if (!needsToStop_.load()) {
                std::lock_guard<std::mutex> lg(eventsListMutex_);
                events_.push_back(newEvent);
                return true;
            }
            return false;
        }

        void joinAllEvents() {
            stop();

            mainThread_.join();

            for (auto& thread : eventsThreads_) {
                thread.join();
            }
        }
    };

}   //  namespace NSolana