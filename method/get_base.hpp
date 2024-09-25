#include <boost/beast/http/dynamic_body.hpp>
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>
#include <optional>
#include <string>

namespace NSolana {

    class TMethodBase {
    public:
        TMethodBase() = default;
        virtual ~TMethodBase() = default;
        //  Need to fill the method name in the request
        virtual constexpr std::string toString() const = 0;
        //  Main method to call
        virtual boost::beast::http::response<boost::beast::http::dynamic_body> call(nlohmann::json&& params);
    protected:
        //  Verification schema for the certain request
        virtual std::optional<std::string> verifyParams(const nlohmann::json& params) = 0;
    private:
        //  Method for rpc2.0 specification support
        nlohmann::json makeRpc2_0JsonWrapper();
    };

}   //  namespace NSolana