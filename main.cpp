#include <random>
#include <iostream>

#include "events/event_handler.hpp"
#include "method/get_methods.hpp"

using namespace boost::asio;
using namespace boost::beast;
using tcp = ip::tcp;
using namespace std::string_literals;

void testClient(const std::string& host, const std::string& port) {
    auto measureTime = [](auto code, size_t count) {
        auto start = std::chrono::high_resolution_clock::now();

        for (size_t counter = 0; counter < count; ++counter)
            code();

        auto finish = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
        std::cout << "Working time: " << duration.count() << " ms" << std::endl;

    };

    NSolana::THttpClient client(host, port);

    auto responseCallback = [](auto&& status_info, auto&& response){};

    //std::cout << "Connection status: " << client.connect().message() << std::endl;
    client.asyncConnect();

    auto mainSyncCode = [&]() {
        auto result = client.request(NSolana::TGetAccountInfo(nlohmann::json::array({
            "vines1vzrYbzLMRdu58ou5XTby4qAqVRLmqo36NKPTg",
            {
                {"encoding", "base58"}
            }
        })));
        std::cout << result << std::endl;
    };

    auto mainAsyncCode = [&]() {
        client.asyncRequest(NSolana::TGetAccountInfo(nlohmann::json::array({
            "vines1vzrYbzLMRdu58ou5XTby4qAqVRLmqo36NKPTg",
            {
                {"encoding", "base58"}
            }
        })),
        [](std::optional<std::string> error, boost::beast::http::response<boost::beast::http::string_body> response) {
            if (error)
                std::cout << *error << std::endl;
            else
                std::cout << response << /*"response was recieved"*/ std::endl;
        });
    };

    measureTime(mainAsyncCode, 1);
}

void testEventHandler(const std::string& host, const std::string& port, size_t solanaDelayMs, size_t statsWindowSizeMs, size_t requestsCount) {
    NSolana::TEventHandler handler;

    auto params = NSolana::TGetAccountInfo(nlohmann::json::array({
            "vines1vzrYbzLMRdu58ou5XTby4qAqVRLmqo36NKPTg",
            {
                {"encoding", "base58"}
            }
    }));

    auto invokeFunctor = std::make_shared<NSolana::TInvokeHandlerFunctor>(host, port, solanaDelayMs, statsWindowSizeMs);
    auto nothingFunctor = std::make_shared<NSolana::TNothingHandlerFunctor>();
    auto errorFunctor = std::make_shared<NSolana::TErrorHandlerFunctor>("Displaying error message");


    handler.registerHandlerFunctor(NSolana::EEventType::INVOKE, invokeFunctor);
    handler.registerHandlerFunctor(NSolana::EEventType::NOTHING, nothingFunctor);
    handler.registerHandlerFunctor(NSolana::EEventType::ERROR, errorFunctor);

    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> distrib(0, 2);

    std::vector<NSolana::EEventType> types = {
        NSolana::EEventType::INVOKE,
        NSolana::EEventType::ERROR,
        NSolana::EEventType::NOTHING
    };


    for (int i = 0; i < requestsCount; ++i) {
        handler.handleEvent(types[distrib(gen)], params);
    }

    handler.join();
}

void testSimpleScheduler() {
    NSolana::TSimpleScheduler<std::function<void()>> sch(2000);

    auto print = []() {
        std::cout << "Event is coming!" << std::endl;
    };

    sch.addEvent(print);

    sch.start();

    sch.addEvent(print);
    sch.addEvent(print);
    sch.addEvent(print);
    sch.addEvent(print);

    sch.joinAllEvents();

    std::cout << std::boolalpha << sch.addEvent(print) << std::endl;
}

void testStats() {
    NSolana::TStats<size_t> stats(5);

    std::vector<std::pair<size_t, double>> range = {
        { 0, 0 },
        { 1, 1 },
        { 2, 2 },
        { 3, 3 },
        { 4, 4 },
        { 5, 5 }
    };

    std::cout << "Standard deviation" << std::endl;
    for (const auto& elem : range) {
        std::cout << stats.getNormalDeviationWithNewElement(elem) << std::endl;
    }
}

int main() {
    const std::string host = "api.devnet.solana.com";
    const std::string port = "80";

    testEventHandler(host, port, 100, 50, 1000);

}