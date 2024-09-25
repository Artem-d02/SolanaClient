#include "get_base.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

// Temporary:
#include <iostream>
#include <nlohmann/json_fwd.hpp>
#include <random>

namespace NSolana {

    boost::beast::http::response<boost::beast::http::dynamic_body> TMethodBase::call(nlohmann::json&& params) {
        namespace beast = boost::beast;
        namespace http = beast::http;
        using tcp = boost::asio::ip::tcp;
        namespace net = boost::asio;
        
        if (auto err = verifyParams(params); err) {
            boost::beast::http::response<boost::beast::http::dynamic_body> bad_result;
            
            bad_result.set(http::field::content_type, "text/plain");
            bad_result.result(http::status::bad_request);
            bad_result.set(http::field::x400_content_type, *err);
            
            return bad_result;
        }

        try {
            net::io_context ioc;
            tcp::socket socket(ioc);

            // Устанавливаем адрес и порт
            const std::string host = "api.devnet.solana.com";
            const std::string port = "80";
            const std::string target = "/post";

            // Создаем JSON-объект
            nlohmann::json args = makeRpc2_0JsonWrapper();
            args["params"] = params;
            std::string body = args.dump();

            // Подключаемся к серверу
            tcp::resolver resolver(ioc);
            auto const results = resolver.resolve(host, port);
            net::connect(socket, results.begin(), results.end());

            // Создаем HTTP POST запрос
            http::request<http::string_body> req(http::verb::post, target, 11);
            req.set(http::field::host, host);
            req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
            req.set(http::field::content_type, "application/json");
            req.body() = body;
            req.prepare_payload();

            // Отправляем запрос
            http::write(socket, req);

            // Получаем ответ
            beast::flat_buffer buffer;
            http::response<http::dynamic_body> res;
            http::read(socket, buffer, res);

            return res;

        } catch (const std::exception& e) {
            std::cerr << "Fatal error: " << e.what() << std::endl;
        }
        return {};
    }

    nlohmann::json TMethodBase::makeRpc2_0JsonWrapper() {
        nlohmann::json wrapper;
        wrapper["jsonrpc"] = "2.0";

        auto now = std::chrono::high_resolution_clock::now();
        std::mt19937 rg(now.time_since_epoch().count());
        wrapper["id"] = rg();
        
        wrapper["method"] = toString();

        return wrapper;
    }

}