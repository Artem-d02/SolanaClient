#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http/string_body.hpp>
#include <string>
#include <functional>

#include "../method/get_base.hpp"

namespace NSolana {

    class THttpClient final {
    private:
        const std::string host_;
        boost::asio::io_context context_;
        boost::asio::ip::tcp::socket socket_;
        boost::asio::deadline_timer timer_;
        boost::asio::ip::tcp::endpoint ep_;
        boost::beast::flat_buffer buffer_{8192};
        boost::beast::http::message<false, boost::beast::http::string_body> msg_;
        bool isConnected_{ false };

        static boost::beast::http::response<boost::beast::http::dynamic_body> createBadResponse(const std::string& message);
    public:
        THttpClient(const std::string& host, const std::string& port);
        boost::system::error_code connect();
        void asyncConnect();
        bool isConnected() const;
        boost::beast::http::response<boost::beast::http::dynamic_body> request(const TMethodBase& method);
        void asyncRequest(
            const TMethodBase& method,
            std::function<void(std::optional<std::string> error, boost::beast::http::response<boost::beast::http::string_body> response)> callback
        );

        //  Temporary
        int StartSendingCounter{ 0 };
        int EndSendingCounter{ 0 };
        int StartRecievingCounter{ 0 };
        int EndRecievingCounter{ 0 };
    };

}   //  namespace NSolana