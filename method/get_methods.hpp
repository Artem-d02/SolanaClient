#pragma once

#include "get_base.hpp"

namespace NSolana {

    class TGetAccountInfo final : public TMethodBase {
    public:
        TGetAccountInfo(const nlohmann::json& params);
        constexpr std::string name() const override;
    protected:
        virtual std::optional<std::string> verifyParams() const override;
    };

    class TGetBalance final : public TMethodBase {
    public:
        TGetBalance(const nlohmann::json& params);
        constexpr std::string name() const override;
    protected:
        virtual std::optional<std::string> verifyParams() const override;
    };

}   //  namespace NSolana