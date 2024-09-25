#include "method/get_methods.hpp"
#include <nlohmann/json_fwd.hpp>
#include <iostream>

int main() {
    NSolana::TGetAccountInfo getAccountInfoMethod;

    std::cout << getAccountInfoMethod.call({}) << std::endl;

    NSolana::TGetBalance getBalance;

    std::cout << getBalance.call(nlohmann::json::array(
        {
            "vines1vzrYbzLMRdu58ou5XTby4qAqVRLmqo36NKPTg",
        }
    )) << std::endl;

    return 0;
}
