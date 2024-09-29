#include "get_base.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <nlohmann/json_fwd.hpp>
#include <random>
#include <utility>

namespace NSolana {

    TMethodBase::TMethodBase(const nlohmann::json& params)
        : params_(params)
    {}

    std::pair<bool, std::string> TMethodBase::getRequestBody() const {        
        if (auto err = verifyParams(); err) {
            return std::make_pair(false, *err);
        }

        // Создаем JSON-объект
        nlohmann::json args = makeRpcV2JsonWrapper();
        args["params"] = params_;
        return std::make_pair(true, args.dump());
    }

    nlohmann::json TMethodBase::makeRpcV2JsonWrapper() const {
        nlohmann::json wrapper;
        wrapper["jsonrpc"] = "2.0";

        auto now = std::chrono::high_resolution_clock::now();
        std::mt19937 rg(now.time_since_epoch().count());
        wrapper["id"] = rg();
        
        wrapper["method"] = name();

        return wrapper;
    }

}