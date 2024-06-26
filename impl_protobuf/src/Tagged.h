#pragma once

template <typename T>
class Tagged_t {
public:
    constexpr Tagged_t(const T& arg) : storage_(arg) {}
    constexpr Tagged_t(const Tagged_t&) = default;
    constexpr Tagged_t(Tagged_t&&) = default;
    constexpr Tagged_t() = default;

    operator const T& () const& { return storage_; }
    operator T () && { return storage_; }
private:
    T storage_;
};
