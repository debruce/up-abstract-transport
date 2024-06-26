#pragma once

#include "Tagged.h"
#include <stdexcept>
#include <string>
#include <variant>

struct BadExpectedAccess : public std::runtime_error {
    template <typename... Args>
    BadExpectedAccess(Args&&... args) : std::runtime_error(std::forward<Args>(args)...) {}
};

using ErrorTag = Tagged_t<std::string>;

template <typename T>
class Expected {
public:
    template <typename... Args>
    constexpr Expected(Args&&... args)
        : storage_(std::forward<Args>(args)...) {}

    constexpr bool has_value() const noexcept {
        return std::holds_alternative<T>(storage_);
    }

    constexpr explicit operator bool() const noexcept {
        return has_value();
    }

    template <class X>
    constexpr T value_or(X&& v) const& noexcept {
        return has_value() ? std::get<T>(storage_) : static_cast<T>(std::forward<X>(v));
    }

    constexpr const T& value() const& {
        if (!has_value())
            throw BadExpectedAccess("Attempt to access value() when unexpected.");
        return std::get<T>(storage_);
    }

    constexpr T value() && {
        if (!has_value())
            throw BadExpectedAccess("Attempt to access value() when unexpected.");
        return std::move(std::get<T>(storage_));
    }

    constexpr const T& operator*() const {
        if (!has_value())
            throw BadExpectedAccess("Attempt to dereference expected value when unexpected.");
        return std::get<T>(storage_);
    }

    constexpr const T* operator->() const {
        if (!has_value())
            throw BadExpectedAccess("Attempt to dereference expected pointer when unexpected.");
        return &std::get<T>(storage_);
    }
private:
    std::variant<T, ErrorTag> storage_;
};
