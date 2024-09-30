#include "client.hpp"

//  Temporary
#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http/impl/write.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/serializer.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/version.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>
#include <optional>
#include <string>

namespace NSolana {

    THttpClient::THttpClient(const std::string& host, const std::string& port)
        : host_(host), context_(), socket_(context_), timer_(context_, boost::posix_time::seconds(5))
    {
        using namespace boost::asio;
        using namespace boost::beast;
        using tcp = ip::tcp;

        ip::tcp::resolver resolver(context_);
        ip::tcp::resolver::query query(host, port);
        ip::tcp::resolver::iterator iter = resolver.resolve( query);
        ep_ = *iter;
        std::cout << ep_.address().to_string() << std::endl;
    }

    boost::system::error_code THttpClient::connect() {
        boost::system::error_code error;

        if (isConnected_)
            return error;   //  success

        error = socket_.connect(ep_, error);
        isConnected_ = !error;
        return error;
    }

    void THttpClient::asyncConnect() {
        if (isConnected_)
            return;

        std::cout << "Starting async request..." << std::endl;
        socket_.async_connect( ep_, [&](const boost::system::error_code& e) {
            if (!e) {
                isConnected_ = true;
                std::cout << "Connection was reached!" << std::endl;
                timer_.cancel();
            }
        });
        std::cout << "Async request was ended" << std::endl;

        context_.run();
    }

    bool THttpClient::isConnected() const {
        return isConnected_;
    }

    boost::beast::http::response<boost::beast::http::dynamic_body> THttpClient::request(const TMethodBase& method) {
        using namespace boost::asio;
        using namespace boost::beast;
        using tcp = ip::tcp;

        auto [isValid, body] = method.getRequestBody();
        
        if (!isValid) {
            return createBadResponse(body);
        }

        const std::string target = "/post";

        // Создаем HTTP POST запрос
        http::request<http::string_body> req(http::verb::post, target, 11);
        req.set(http::field::host, host_);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set(http::field::content_type, "application/json");
        req.body() = body;
        req.prepare_payload();

        // Отправляем запрос
        http::write(socket_, req);

        // Получаем ответ
        boost::beast::flat_buffer buffer;
        http::response<http::dynamic_body> res;
        http::read(socket_, buffer, res);

        return res;
    }

    void THttpClient::asyncRequest(
        const TMethodBase& method,
        std::function<void(std::optional<std::string> error, boost::beast::http::response<boost::beast::http::string_body> response)> callback
    ) {
        using namespace boost::asio;
        using namespace boost::beast;
        using tcp = ip::tcp;

        auto [isValid, body] = method.getRequestBody();
        
        if (!isValid) {
            callback(std::make_optional("Parameters error: " + body), {});
            return;
        }

        const std::string target = "/post";

        // Создаем HTTP POST запрос
        http::request<http::string_body> req(http::verb::post, target, 11);
        req.set(http::field::host, host_);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set(http::field::content_type, "application/json");
        req.body() = body;
        req.prepare_payload();

        context_.restart(); //  TODO

        auto createTimeOutHandler = [&](const std::string& timeOutMsg) {
            return [=, this](const boost::system::error_code& e) {
                if (e == boost::asio::error::operation_aborted) {
                    return;
                }
                socket_.cancel();
                callback(std::make_optional(timeOutMsg), {});
            };
        };

        http::serializer<true, http::string_body> serializer(req);

        std::cout << "Start sending #" << StartSendingCounter++ << std::endl;    //  tmp
        http::async_write(socket_, serializer, [&](const error_code& error, std::size_t bytes_transferred){
            if (error) {
                callback(std::make_optional("Async write error: " + error.message()), {});
                return;
            }
            timer_.cancel();

            std::cout << "End sending #" << EndSendingCounter++ << std::endl;    //  tmp
            
            http::parser<false, http::string_body> p;

            std::cout << "Start recieving #" << StartRecievingCounter++ << std::endl;  //  tmp
            http::async_read(socket_, buffer_, msg_, [&](error_code const& error, std::size_t bytes_transferred){
                if (error) {
                    callback(std::make_optional("Async read error: " + error.message()), {});
                    return;
                }
                timer_.cancel();
                std::cout << "End recieving #" << EndRecievingCounter++ << std::endl;  //  tmp
                callback(std::nullopt, msg_);
            });

            timer_.async_wait(createTimeOutHandler("Timeout during reading was reached"));

        });

        timer_.async_wait(createTimeOutHandler("Timeout during writing was reached"));

        context_.run();
    }

    boost::beast::http::response<boost::beast::http::dynamic_body> THttpClient::createBadResponse(const std::string& message) {
        boost::beast::http::response<boost::beast::http::dynamic_body> bad_result;
            
            bad_result.set(boost::beast::http::field::content_type, "text/plain");
            bad_result.result(boost::beast::http::status::bad_request);
            bad_result.set(boost::beast::http::field::x400_content_type, message);
            
            return bad_result;
    }

}   //  namespace NSolana