#include "client/client.hpp"
#include "method/get_methods.hpp"
#include <boost/beast/http/basic_parser.hpp>
#include <boost/beast/http/parser.hpp>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/version.hpp>

#include "events/event_handler.hpp"

//  Temporary
#include <iostream>
#include <nlohmann/json_fwd.hpp>

using namespace boost::asio;
using namespace boost::beast;
using tcp = ip::tcp;
using namespace std::string_literals;

int main() {
    const std::string host = "api.devnet.solana.com";
    const std::string port = "80";

    // auto measureTime = [](auto code, size_t count) {
    //     auto start = std::chrono::high_resolution_clock::now();

    //     for (size_t counter = 0; counter < count; ++counter)
    //         code();

    //     auto finish = std::chrono::high_resolution_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    //     std::cout << "Working time: " << duration.count() << " ms" << std::endl;

    // };

    // NSolana::THttpClient client(host, port);

    // auto responseCallback = [](auto&& status_info, auto&& response){};

    // //std::cout << "Connection status: " << client.connect().message() << std::endl;
    // client.asyncConnect();

    // auto mainSyncCode = [&]() {
    //     auto result = client.request(NSolana::TGetAccountInfo(nlohmann::json::array({
    //         "vines1vzrYbzLMRdu58ou5XTby4qAqVRLmqo36NKPTg",
    //         {
    //             {"encoding", "base58"}
    //         }
    //     })));
    //     std::cout << result << std::endl;
    // };

    // auto mainAsyncCode = [&]() {
    //     client.asyncRequest(NSolana::TGetAccountInfo(nlohmann::json::array({
    //         "vines1vzrYbzLMRdu58ou5XTby4qAqVRLmqo36NKPTg",
    //         {
    //             {"encoding", "base58"}
    //         }
    //     })),
    //     [](std::optional<std::string> error, boost::beast::http::response<boost::beast::http::string_body> response) {
    //         if (error)
    //             std::cout << *error << std::endl;
    //         else
    //             std::cout << response << /*"response was recieved"*/ std::endl;
    //     });
    // };

    // measureTime(mainAsyncCode, 1);

    NSolana::TEventHandler handler(host, port);

    auto params = NSolana::TGetAccountInfo(nlohmann::json::array({
            "vines1vzrYbzLMRdu58ou5XTby4qAqVRLmqo36NKPTg",
            {
                {"encoding", "base58"}
            }
    }));

    //  TODO: fix ratelimit exceed error
    for (int i = 0; i < 2; ++i)
        handler.handleEvent(NSolana::EEventType::INVOKE, &params);

    handler.join();
}