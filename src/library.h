#pragma once

#include <string>
#include <vector>
#include <array>
#include <variant>
#include <optional>
#include <map>
#include <complex>
#include <tuple>

#include <glaze/glaze.hpp>

// ----------------------------------------------------------------------------
// we can use enums
// ----------------------------------------------------------------------------
enum class E {
    E1,
    E2,
    E3,
};

// ----------------------------------------------------------------------------
// nested structs work out of the box
// ----------------------------------------------------------------------------
struct Nested {
    std::string nested = "nested_value";
    bool operator==(const Nested& other) const = default; // only for testing purposes
};

// ----------------------------------------------------------------------------
// variants? no problem
// ----------------------------------------------------------------------------
struct A {
    int a;
    bool operator==(const A& other) const noexcept = default; // only for testing purposes

    A operator+(A other) const noexcept {
        return A{a + other.a};
    }
};

struct B {
    float b;
    bool operator==(const B& other) const noexcept = default; // only for testing purposes
};

using AorB = std::variant<A, B>;

// NOTE: if the variant composition is ambiguous, we need to add tags
// in this case both A and B could be constructed from a single number
// so we add a tag to disambiguate them
template <>
struct glz::meta<AorB> {
    static constexpr std::string_view tag = "_tag";
};

// ----------------------------------------------------------------------------
// main struct with various types
// ----------------------------------------------------------------------------

struct X {

    // all sorts of types
    int i = 1;
    float f = 2.0f;
    std::string str = "3";
    std::vector<int> vec = {4, 5, 6};
    std::array<double, 3> arr = {7.0, 8.0, 9.0};
    std::variant<int, std::string> var = "10";
    std::optional<float> opt = 11.0f;
    std::map<std::string, int> mp = {{"12", 12}, {"13", 13}};
    std::complex<double> comp = {14.0, 15.0};
    std::tuple<int, std::string, float> tup = {16, "17", 18.0f};

    // nested struct
    Nested nested;

    // enums
    E e = E::E2;

    // variant alternatives that are ambiguous without tags
    AorB aOrB = B{};

    bool operator==(const X& other) const noexcept = default; // only for testing purposes
};
