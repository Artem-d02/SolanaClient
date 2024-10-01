#include "event_handler.hpp"
#include "../method/get_methods.hpp"
#include <iostream>
#include <memory>

namespace NSolana {

    std::string eventTypeToString(EEventType e) {
        switch (e) {
        case (EEventType::INVOKE):
            return "INVOKE";
        case (EEventType::NOTHING):
            return "NOTHING";
        case (EEventType::ERROR):
            return "ERROR";
        default:
            return "UNKNOWN";
        }
    }

    TInvokeHandlerFunctor::TInvokeHandlerFunctor(const std::string& host, const std::string& port, size_t msRequestsInterval)
        : client_(host, port), scheduler_(msRequestsInterval)
    {
        auto err = client_.connect();
        if (err)
            std::cout << err.message() << std::endl;
        scheduler_.start();
    }

    void TInvokeHandlerFunctor::call(const TMethodBase& method) {
        scheduler_.addEvent([&]() {
            std::cout << client_.request(method) << std::endl;
        });
    }

    void TInvokeHandlerFunctor::join() {
        scheduler_.joinAllEvents();
    }

    void TEventHandler::handleEvent(EEventType event, const TMethodBase& method) {

        if (!handlers_.contains(event)) {
            std::cout << "Error: handler for event <" << eventTypeToString(event) << "> is not registered" << std::endl;
        }
        handlers_[event]->call(method);

    }

    void TEventHandler::registerHandlerFunctor(EEventType type, std::unique_ptr<IBaseHandlerFunctor>&& functorPtr) {
        handlers_[type] = std::move(functorPtr);
    }

    void TEventHandler::join() {
        for (auto& [_, handler_] : handlers_) {
            handler_->join();
        }
    }

}   //  namespace NSolana