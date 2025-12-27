#include <print>
#include <glaze/glaze.hpp>

#include "library.h"
#include "migrate.h"

void test_library() {
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
}

void test_migrate() {
    constexpr auto opts = glz::opts{.prettify = true, .indentation_width = 4};

    std::string json_version1 = R"json({
    "format": {
        "version": {
            "major": 1,
            "minor": 0,
            "patch": 0
        }
    },
    "payload": {
        "unchanged": 10,
        "changed_default_value": 20,
        "renamed": 30,
        "some_var": 40,
        "obsolete": 50,
        "reshaped": 70
    }
})json";

    // first we parse the envelope, containing only the version and raw payload
    auto envelope = glz::read_json<Envelope>(json_version1);

    if (!envelope) {
        std::println("Error deserializing JSON to Envelope: {}", glz::format_error(envelope.error(), json_version1));
        std::abort();
    }

    // now we can parse the payload based on the version
    auto parse_payload = [&]() -> CurrentVersion {
        if (envelope->format.version.major == 1) {
            auto old_version = glz::read_json<Version1>(envelope->payload.str);
            if (!old_version) {
                std::println("Error deserializing payload to Version1: {}", glz::format_error(old_version.error(), envelope->payload.str));
                std::abort();
            }
            return migrate(*old_version);
        }
        else if (envelope->format.version.major == 2) {
            auto old_version = glz::read_json<Version2>(envelope->payload.str);
            if (!old_version) {
                std::println("Error deserializing payload to Version2: {}", glz::format_error(old_version.error(), envelope->payload.str));
                std::abort();
            }
            return migrate(*old_version);
        }
        else {
            std::println("Unsupported version: {}.{}.{}", envelope->format.version.major, envelope->format.version.minor, envelope->format.version.patch);
            std::abort();
        }
    };
    CurrentVersion current_version = parse_payload();

    // write out the current version to json
    auto json_version2 = glz::write<opts>(current_version);

    if (!json_version2) {
        std::println("Error serializing CurrentVersion to JSON: {}", glz::format_error(json_version2.error(), ""));
        std::abort();
    }

    std::println("Serialized CurrentVersion to JSON:\n{}", *json_version2);
}

int main() {
    test_library();
    std::println();
    std::println();
    test_migrate();

    return 0;
}
