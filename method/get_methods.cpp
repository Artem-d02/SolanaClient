#include "get_methods.hpp"
#include "constants.hpp"
#include <format>
#include <string>

namespace NSolana {

    constexpr std::string TGetAccountInfo::toString() const {
        return "getAccountInfo";
    }

    std::optional<std::string>  TGetAccountInfo::verifyParams(const nlohmann::json& params) {
        constexpr std::string_view errorTemplate = "[{}]: {} - {}";   //  [method]: common_description - details
        auto methodName = toString();

        if (!params.is_array())
            return std::make_optional(std::format(errorTemplate, methodName, TConstants::ERROR_TYPE, TConstants::SHOULD_BE_AN_ARRAY));
        
        if (params.size() == 0)
            return std::make_optional(std::format(errorTemplate, methodName, TConstants::ERROR_TYPE, TConstants::SHOULD_BE_NOT_EMPTY_ARRAY));

        if (!params[0].is_string())
            return std::make_optional(std::format(errorTemplate, methodName, TConstants::ERROR_TYPE, TConstants::SHOULD_BE_A_STRING));

        return std::nullopt;
    }

    constexpr std::string TGetBalance::toString() const {
        return "getBalance";
    }

    std::optional<std::string>  TGetBalance::verifyParams(const nlohmann::json& params) {
        auto methodName = toString();
        constexpr std::string_view errorTemplate = "[{}]: {} - {}";   //  [method]: common_description - details

        if (!params.is_array())
            return std::make_optional(std::format(errorTemplate, methodName, TConstants::ERROR_TYPE, TConstants::SHOULD_BE_AN_ARRAY));
        
        if (params.size() == 0)
            return std::make_optional(std::format(errorTemplate, methodName, TConstants::ERROR_TYPE, TConstants::SHOULD_BE_NOT_EMPTY_ARRAY));

        return std::nullopt;
    }

}   //  namespace NSolana