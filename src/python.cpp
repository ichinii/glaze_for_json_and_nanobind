#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/variant.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/complex.h>
#include <nanobind/stl/tuple.h>
#include <glaze/glaze.hpp>
#include <tuple> // Still needed for std::decay_t, etc.
#include <print>

namespace nb = nanobind;
using namespace nb::literals;

// ---------------------------------------------------------------------------
// Auto-generate common class methods for nanobind bindings
// ---------------------------------------------------------------------------

template <typename T>
nb::class_<T> nb_class_common(nb::module_& m, const char* name) {
    auto cls = nb::class_<T>(m, name);

    // ---------------------------------------------------------------------------
    // Default constructor
    // TODO: improve to support more constructors, e.g., with default and optional args
    // ---------------------------------------------------------------------------

    if constexpr (std::is_default_constructible_v<T>) {
        cls.def(nb::init<>());
    }

    // ---------------------------------------------------------------------------
    // Auto-generate common dunder methods if supported by T
    // ---------------------------------------------------------------------------

#ifndef NDEBUG
    std::println("Defining common dunder methods for type `{}`", glz::name_v<T>);
#define DBG_OP(op_name) std::println("Defining `{}`", op_name);
#else
#define DBG_OP(op_name)
#endif


    DBG_OP("__repr__");
    cls.def("__repr__", [](const T& a) {
        return glz::write_json(a).value_or("<error>");
    });

    if constexpr (requires { static_cast<std::string>(std::declval<T>()); }) {
        DBG_OP("__str__ (using static_cast<std::string>(self))");
        cls.def("__str__", [](const T& a) {
            return static_cast<std::string>(a);
        });
    } else if constexpr (requires { std::to_string(std::declval<T>()); }) {
        DBG_OP("__str__ (using std::to_string(self))");
        cls.def("__str__", [](const T& a) {
            return std::to_string(a);
        });
    } else if constexpr (requires { std::declval<T>().to_string(); }) {
        DBG_OP("__str__ (using self.to_string())");
        cls.def("__str__", [](const T& a) {
            return a.to_string();
        });
    } else {
        // not required to have __str__ since __repr__ is defined and python will fall back to it
    }

    if constexpr (requires { std::hash<T>{}(std::declval<T>()); }) {
        DBG_OP("__hash__");
        cls.def("__hash__", [](const T& a) {
            return std::hash<T>{}(a);
        });
    }

    if constexpr (requires { std::declval<T>() == std::declval<T>(); }) {
        DBG_OP("__eq__");
        cls.def("__eq__", [](const T& a, const T& b) {
            return a == b;
        });
    }

    if constexpr (requires { std::declval<T>() < std::declval<T>(); }) {
        DBG_OP("__lt__");
        cls.def("__lt__", [](const T& a, const T& b) {
            return a < b;
        });
    }

    if constexpr (requires { std::declval<T>() <= std::declval<T>(); }) {
        DBG_OP("__le__");
        cls.def("__le__", [](const T& a, const T& b) {
            return a <= b;
        });
    }

    if constexpr (requires { std::declval<T>() > std::declval<T>(); }) {
        DBG_OP("__gt__");
        cls.def("__gt__", [](const T& a, const T& b) {
            return a > b;
        });
    }

    if constexpr (requires { std::declval<T>() >= std::declval<T>(); }) {
        DBG_OP("__ge__");
        cls.def("__ge__", [](const T& a, const T& b) {
            return a >= b;
        });
    }

    if constexpr (requires { std::declval<T>() + std::declval<T>(); }) {
        DBG_OP("__add__");
        cls.def("__add__", [](const T& a, const T& b) {
            return a + b;
        });
    }

    if constexpr (requires { std::declval<T>() - std::declval<T>(); }) {
        DBG_OP("__sub__");
        cls.def("__sub__", [](const T& a, const T& b) {
            return a - b;
        });
    }

    if constexpr (requires { std::declval<T>() * std::declval<T>(); }) {
        DBG_OP("__mul__");
        cls.def("__mul__", [](const T& a, const T& b) {
            return a * b;
        });
    }

    if constexpr (requires { std::declval<T>() / std::declval<T>(); }) {
        DBG_OP("__truediv__");
        cls.def("__truediv__", [](const T& a, const T& b) {
            return a / b;
        });
    }

    if constexpr (requires { std::declval<T>() % std::declval<T>(); }) {
        DBG_OP("__mod__");
        cls.def("__mod__", [](const T& a, const T& b) {
            return a % b;
        });
    }

    if constexpr (requires { std::declval<T>() += std::declval<T>(); }) {
        DBG_OP("__iadd__");
        cls.def("__iadd__", [](T& a, const T& b) {
            a += b;
            return a;
        });
    }

    if constexpr (requires { std::declval<T>() -= std::declval<T>(); }) {
        DBG_OP("__isub__");
        cls.def("__isub__", [](T& a, const T& b) {
            a -= b;
            return a;
        });
    }

    if constexpr (requires { std::declval<T>() *= std::declval<T>(); }) {
        DBG_OP("__imul__");
        cls.def("__imul__", [](T& a, const T& b) {
            a *= b;
            return a;
        });
    }

    if constexpr (requires { std::declval<T>() /= std::declval<T>(); }) {
        DBG_OP("__itruediv__");
        cls.def("__itruediv__", [](T& a, const T& b) {
            a /= b;
            return a;
        });
    }

    if constexpr (requires { std::declval<T>() %= std::declval<T>(); }) {
        DBG_OP("__imod__");
        cls.def("__imod__", [](T& a, const T& b) {
            a %= b;
            return a;
        });
    }

    if constexpr (requires { -std::declval<T>(); }) {
        DBG_OP("__neg__");
        cls.def("__neg__", [](const T& a) {
            return -a;
        });
    }

    if constexpr (requires { +std::declval<T>(); }) {
        DBG_OP("__pos__");
        cls.def("__pos__", [](const T& a) {
            return +a;
        });
    }

    if constexpr (requires { ~std::declval<T>(); }) {
        DBG_OP("__invert__");
        cls.def("__invert__", [](const T& a) {
            return ~a;
        });
    }

    if constexpr (requires { ++std::declval<T>(); }) {
        DBG_OP("__pre_inc__");
        cls.def("__pre_inc__", [](T& a) {
            return ++a;
        });
    }

    if constexpr (requires { --std::declval<T>(); }) {
        DBG_OP("__pre_dec__");
        cls.def("__pre_dec__", [](T& a) {
            return --a;
        });
    }

    if constexpr (requires { std::declval<T>()++; }) {
        DBG_OP("__post_inc__");
        cls.def("__post_inc__", [](T& a) {
            return a++;
        });
    }

    if constexpr (requires { std::declval<T>()--; }) {
        DBG_OP("__post_dec__");
        cls.def("__post_dec__", [](T& a) {
            return a--;
        });
    }

    if constexpr (requires { std::declval<T>()(); }) {
        DBG_OP("__call__");
        cls.def("__call__", [](T& a) {
            return a();
        });
    }

    if constexpr (requires { std::declval<const T>()(); }) {
        DBG_OP("__call__ const");
        cls.def("__call__", [](const T& a) {
            return a();
        });
    }

    if constexpr (requires { std::declval<T>().size(); }) {
        DBG_OP("__len__");
        cls.def("__len__", [](const T& a) {
            return a.size();
        });
    }

    if constexpr (requires { std::declval<T>().empty(); }) {
        DBG_OP("__bool__ (using .empty())");
        cls.def("__bool__", [](const T& a) {
            return not a.empty();
        });
    }

    if constexpr (requires { static_cast<bool>(std::declval<T>()); }) {
        DBG_OP("__bool__ (using static_cast<bool>(self)");
        cls.def("__bool__", [](const T& a) {
            return static_cast<bool>(a);
        });
    }

#undef DBG_OP

    // ---------------------------------------------------------------------------
    // Auto-generate properties for all fields via Glaze reflection
    // ---------------------------------------------------------------------------

    // Number of fields
    constexpr auto N = glz::reflect<T>::size;

    // Iterate 0..N-1
    [&]<size_t... I>(std::index_sequence<I...>) {
        (
            [&]() {
                // 1. Get Field Name
                constexpr auto key_sv = glz::reflect<T>::keys[I];

                // 2. Get Field Type safely via to_tie
                // glz::to_tie returns a glz::tuple (or similar) of references
                using TupleType = decltype(glz::to_tie(std::declval<T>()));

                // Use glz::tuple_element_t (or equivalent) if available,
                // OR simpler: decltype(glz::get<I>(std::declval<TupleType>()))
                // The element is a reference (e.g., int&), so decay it to get 'int'.
                using ElementRefType = decltype(glz::get<I>(std::declval<TupleType>()));
                using FieldType = std::decay_t<ElementRefType>;

                // 3. Register Property
                cls.def_prop_rw(
                    key_sv.data(),

                    // Getter
                    [](const T& self) -> const FieldType& {
                        // Create a tuple of references from 'self'
                        auto t = glz::to_tie(const_cast<T&>(self));
                        // Use glz::get to access the element from the glaze tuple
                        return glz::get<I>(t);
                    },
                    // Setter
                    [](T& self, const FieldType& val) {
                        auto t = glz::to_tie(self);
                        glz::get<I>(t) = val;
                    }
                );
            }(),
            ...
        );
    }(std::make_index_sequence<N>{});

    return cls;
}

// ---------------------------------------------------------------------------
// Module Definition
// ---------------------------------------------------------------------------

#include "library.h"

NB_MODULE(test_py, m) {
    m.doc() = "Nanobind bindings for Glaze reflected types";

    nb::enum_<E>(m, "E")
        .value("E1", E::E1)
        .value("E2", E::E2)
        .value("E3", E::E3)
        .export_values();

    nb_class_common<Nested>(m, "Nested");
    nb_class_common<A>(m, "A");
    nb_class_common<B>(m, "B");
    nb_class_common<X>(m, "X");
}

