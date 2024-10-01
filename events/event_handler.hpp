#pragma once

#include "../client/client.hpp"
#include "scheduler.hpp"
#include <thread>
#include <mutex>
#include <unordered_map>
#include <map>
#include <memory>

namespace NSolana {

    enum class EEventType {
        INVOKE,
        NOTHING,
        ERROR
    };

    std::string eventTypeToString(EEventType e);

    class IBaseHandlerFunctor {
    public:
        virtual ~IBaseHandlerFunctor() = default;
        virtual void call(const TMethodBase& method) = 0;
        virtual void join() = 0;
    };

    class TInvokeHandlerFunctor : public IBaseHandlerFunctor {
    private:
        THttpClient client_;
        TSimpleScheduler<std::function<void()>> scheduler_;
        std::map<size_t, std::pair<boost::beast::http::response<boost::beast::http::string_body>, std::chrono::milliseconds>> responses_;
        std::mutex responsesMutex_;
    public:
        TInvokeHandlerFunctor(const std::string& host, const std::string& port, size_t msRequestsInterval);
        void call(const TMethodBase& method) override;
        void join() override;
        const std::map<size_t, std::pair<boost::beast::http::response<boost::beast::http::string_body>, std::chrono::milliseconds>>& getResponses() const noexcept;
    };

    class TNothingHandlerFunctor : public IBaseHandlerFunctor {
    public:
        TNothingHandlerFunctor() = default;
        void call(const TMethodBase& method) override;
        void join() override;
    };

    class TErrorHandlerFunctor : public IBaseHandlerFunctor {
    private:
        std::string errorMsg_;
    public:
        TErrorHandlerFunctor(const std::string errorMsg);
        void call(const TMethodBase& method) override;
        void join() override;
    };

    class TEventHandler final {
    private:
        std::unordered_map<EEventType, std::shared_ptr<IBaseHandlerFunctor>> handlers_;
    public:
        TEventHandler() = default;
        void handleEvent(EEventType event, const TMethodBase& method);
        void registerHandlerFunctor(EEventType type, std::shared_ptr<IBaseHandlerFunctor> functorPtr);
        void join();
    };

}   //  namespace NSolana