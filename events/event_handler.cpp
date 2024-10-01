#include "event_handler.hpp"
#include "../method/get_methods.hpp"
#include <iostream>
#include <memory>
#include <boost/beast/core.hpp>

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
            auto start = std::chrono::high_resolution_clock::now();
            auto response = client_.request(method);
            auto finish = std::chrono::high_resolution_clock::now();
            auto latency = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);

            nlohmann::json responseBodyJson = nlohmann::json::parse(boost::beast::buffers_to_string(response.body().data()));

            auto result = responseBodyJson.find("result");
            if (result == responseBodyJson.end()) {
                std::cout << "Error: field <result> is not in response" << std::endl;
                return;
            }

            auto context = result->find("context");
            if (context == result->end()) {
                std::cout << "Error: field <context> is not in response" << std::endl;
                return;
            }

            auto slot = context->find("slot");
            if (slot == context->end()) {
                std::cout << "Error: field <slot> is not in response" << std::endl;
                return;
            }

            size_t slotVal = slot->get<size_t>();
            {
                std::lock_guard<std::mutex> lg(responsesMutex_);
                if (!responses_.contains(slotVal)) {
                    std::cout << "Getting slot = " << slotVal << " with latency = " << latency.count() << std::endl;
                    responses_.emplace(slotVal, std::make_pair(response, latency));
                }
            }

        });
    }

    void TInvokeHandlerFunctor::join() {
        scheduler_.joinAllEvents();
    }

    const std::map<size_t, std::pair<boost::beast::http::response<boost::beast::http::string_body>, std::chrono::milliseconds>>& TInvokeHandlerFunctor::getResponses() const noexcept {
        return responses_;
    }

    void TNothingHandlerFunctor::call(const TMethodBase& method) {
        //  Do nothing
    }

    void TNothingHandlerFunctor::join() {
        //  Do nothing
    }

    TErrorHandlerFunctor::TErrorHandlerFunctor(const std::string errorMsg)
        : errorMsg_(errorMsg)
    {}

    void TErrorHandlerFunctor::call(const TMethodBase& method) {
        std::cout << errorMsg_ << std::endl;
    }

    void TErrorHandlerFunctor::join() {
        //  Do nothing
    }

    void TEventHandler::handleEvent(EEventType event, const TMethodBase& method) {

        if (!handlers_.contains(event)) {
            std::cout << "Error: handler for event <" << eventTypeToString(event) << "> is not registered" << std::endl;
        }
        handlers_[event]->call(method);

    }

    void TEventHandler::registerHandlerFunctor(EEventType type, std::shared_ptr<IBaseHandlerFunctor> functorPtr) {
        handlers_[type] = functorPtr;
    }

    void TEventHandler::join() {
        for (auto& [_, handler_] : handlers_) {
            handler_->join();
        }
    }

}   //  namespace NSolana