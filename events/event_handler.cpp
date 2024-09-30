#include "event_handler.hpp"
#include "../method/get_methods.hpp"
#include <iostream>
#include <memory>

namespace NSolana {

    TEventHandler::TEventHandler(const std::string& host, const std::string& port)
        : client_(host, port)
    {
        auto err = client_.connect();
        if (err)
            std::cout << err.message() << std::endl;
    }

    void TEventHandler::handleEvent(EEventType event, TMethodBase* method) {
        std::cout << "Input object in handler: " << typeid(method).name() << std::endl;
        
        threads_.emplace_back(
            [&](TMethodBase* method) mutable {
                std::cout << "Input object in thread: " << typeid(method).name() << std::endl;
                std::cout << client_.request(*method) << std::endl;
            },
            method
        );

    }

    void TEventHandler::join() {
        for (auto& thread : threads_) {
            thread.join();
        }
    }

}   //  namespace NSolana