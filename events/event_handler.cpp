#include "event_handler.hpp"
#include "../method/get_methods.hpp"
#include <iostream>
#include <memory>

namespace NSolana {

    TEventHandler::TEventHandler(const std::string& host, const std::string& port, size_t msRequestsInterval)
        : client_(host, port), scheduler_(msRequestsInterval)
    {
        auto err = client_.connect();
        if (err)
            std::cout << err.message() << std::endl;
        scheduler_.start();
    }

    void TEventHandler::handleEvent(EEventType event, const TMethodBase& method) {

        std::function<void()> eventFunc([&]() {
            std::cout << client_.request(method) << std::endl;
        });

        scheduler_.addEvent(eventFunc);

    }

    void TEventHandler::join() {
        scheduler_.joinAllEvents();
    }

}   //  namespace NSolana