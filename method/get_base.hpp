#pragma once

#include <boost/beast/http/dynamic_body.hpp>
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>
#include <optional>
#include <string>

namespace NSolana {

    class TMethodBase {
    public:
        TMethodBase(const nlohmann::json& params);
        virtual ~TMethodBase() = default;
        //  Need to fill the method name in the request
        virtual constexpr std::string name() const = 0;
        //  Method to get the created request body
        std::pair<bool, std::string> getRequestBody() const;   //  return <isValid, body>
    protected:
        //  Verification schema for the certain request
        virtual std::optional<std::string> verifyParams() const = 0;
    private:
        //  Method for rpc2.0 specification support
        nlohmann::json makeRpcV2JsonWrapper() const;
    protected:
        const nlohmann::json params_;
    };

}   //  namespace NSolana