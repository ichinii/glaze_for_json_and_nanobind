#include <print>
#include <glaze/glaze.hpp>

#include "library.h"

int main() {
    constexpr auto opts = glz::opts{.prettify = true, .indentation_width = 4};

    X x;
    auto json_result = glz::write<opts>(x);

    if (!json_result) {
        std::println("Error serializing X to JSON: {}", glz::format_error(json_result.error(), ""));
        std::abort();
    }

    std::println("Serialized X to JSON:\n{}", *json_result);

    auto x_result = glz::read_json<X>(*json_result);

    if (!x_result) {
        std::println("Error deserializing JSON to X: {}", glz::format_error(x_result.error(), *json_result));
        std::abort();
    }

    if (x != *x_result) {
        std::println("Deserialized X does not match the original.");
        std::abort();
    }

    std::println("Deserialized JSON back to X successfully.");

    return 0;
}
