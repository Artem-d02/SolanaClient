#include "get_base.hpp"

namespace NSolana {

    class TGetAccountInfo final : public TMethodBase {
    public:
        TGetAccountInfo() = default;
        constexpr std::string toString() const override;
    protected:
        virtual std::optional<std::string> verifyParams(const nlohmann::json& params) override;
    };

    class TGetBalance final : public TMethodBase {
    public:
        TGetBalance() = default;
        constexpr std::string toString() const override;
    protected:
        virtual std::optional<std::string> verifyParams(const nlohmann::json& params) override;
    };

}   //  namespace NSolana