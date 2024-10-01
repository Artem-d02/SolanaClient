#pragma once

#include "../client/client.hpp"
#include "scheduler.hpp"
#include <thread>
#include <unordered_map>
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
    public:
        TInvokeHandlerFunctor(const std::string& host, const std::string& port, size_t msRequestsInterval);
        void call(const TMethodBase& method) override;
        void join() override;
    };

    class TEventHandler final {
    private:
        std::unordered_map<EEventType, std::unique_ptr<IBaseHandlerFunctor>> handlers_;
    public:
        TEventHandler() = default;
        void handleEvent(EEventType event, const TMethodBase& method);
        void registerHandlerFunctor(EEventType type, std::unique_ptr<IBaseHandlerFunctor>&& functorPtr);
        void join();
    };

}   //  namespace NSolana