#include "../client/client.hpp"
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
        std::vector<std::thread> threads_;
    public:
        TEventHandler(const std::string& host, const std::string& port);
        void handleEvent(EEventType event, TMethodBase* method);
        void join();
    };

}   //  namespace NSolana