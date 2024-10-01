#include "../client/client.hpp"
#include "scheduler.hpp"
#include <thread>

namespace NSolana {

    enum class EEventType {
        INVOKE,
        NOTHING,
        ERROR
    };

    class TEventHandler final {
    private:
        THttpClient client_;
        TSimpleScheduler<std::function<void()>> scheduler_;
    public:
        TEventHandler(const std::string& host, const std::string& port, size_t msRequestsInterval);
        void handleEvent(EEventType event, const TMethodBase& method);
        void join();
    };

}   //  namespace NSolana