#include "get_methods.hpp"
#include "constants.hpp"
#include "get_base.hpp"
#include <format>
#include <string>

namespace NSolana {

    TGetAccountInfo::TGetAccountInfo(const nlohmann::json& params)
        : TMethodBase(params)
    {}

    constexpr std::string TGetAccountInfo::name() const {
        return "getAccountInfo";
    }

    std::optional<std::string>  TGetAccountInfo::verifyParams() const {
        constexpr std::string_view errorTemplate = "[{}]: {} - {}";   //  [method]: common_description - details
        auto methodName = name();

        if (!params_.is_array())
            return std::make_optional(std::format(errorTemplate, methodName, TConstants::ERROR_TYPE, TConstants::SHOULD_BE_AN_ARRAY));
        
        if (params_.size() == 0)
            return std::make_optional(std::format(errorTemplate, methodName, TConstants::ERROR_TYPE, TConstants::SHOULD_BE_NOT_EMPTY_ARRAY));

        if (!params_[0].is_string())
            return std::make_optional(std::format(errorTemplate, methodName, TConstants::ERROR_TYPE, TConstants::SHOULD_BE_A_STRING));

        return std::nullopt;
    }

    TGetBalance::TGetBalance(const nlohmann::json& params)
        : TMethodBase(params)
    {}

    constexpr std::string TGetBalance::name() const {
        return "getBalance";
    }

    std::optional<std::string>  TGetBalance::verifyParams() const {
        auto methodName = name();
        constexpr std::string_view errorTemplate = "[{}]: {} - {}";   //  [method]: common_description - details

        if (!params_.is_array())
            return std::make_optional(std::format(errorTemplate, methodName, TConstants::ERROR_TYPE, TConstants::SHOULD_BE_AN_ARRAY));
        
        if (params_.size() == 0)
            return std::make_optional(std::format(errorTemplate, methodName, TConstants::ERROR_TYPE, TConstants::SHOULD_BE_NOT_EMPTY_ARRAY));

        return std::nullopt;
    }

}   //  namespace NSolana