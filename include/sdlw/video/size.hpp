#pragma once

#include <type_traits>

namespace sdlw::video {

struct size {
    int w;
    int h;
};

// Comparison

constexpr
bool
operator==(const size& lhs, const size& rhs) noexcept
{
    return lhs.w == rhs.w && lhs.h == rhs.h;
}

constexpr
bool
operator!=(const size& lhs, const size& rhs) noexcept
{
    return !(lhs == rhs);
}

constexpr
bool
operator<(const size& lhs, const size& rhs) noexcept
{
    return lhs.w < rhs.w && lhs.h < rhs.h;
}

constexpr
bool
operator>(const size& lhs, const size& rhs) noexcept
{
    return rhs < lhs;
}

constexpr
bool
operator<=(const size& lhs, const size& rhs) noexcept
{
    return !(rhs < lhs);
}

constexpr
bool
operator>=(const size& lhs, const size& rhs) noexcept
{
    return !(lhs < rhs);
}

// Arithmetic (unary)

constexpr
size
operator-(const size& s) noexcept
{
    return size{-s.w, -s.h};
}

// Arithmetic (binary, symmetrical)

constexpr
size
operator+(const size& lhs, const size& rhs) noexcept
{
    return size{lhs.w + rhs.w, lhs.h + rhs.h};
}

constexpr
size
operator-(const size& lhs, const size& rhs) noexcept
{
    return size{lhs.w - rhs.w, lhs.h - rhs.h};
}

constexpr
size
operator*(const size& lhs, const size& rhs) noexcept
{
    return size{lhs.w * rhs.w, lhs.h * rhs.h};
}

constexpr
size
operator/(const size& lhs, const size& rhs) noexcept
{
    return size{lhs.w / rhs.w, lhs.h / rhs.h};
}

// Arithmetic (assingment, symmetrical)

constexpr
size&
operator+=(size& lhs, const size& rhs) noexcept
{
    lhs = lhs + rhs;
    return lhs;
}

constexpr
size&
operator-=(size& lhs, const size& rhs) noexcept
{
    lhs = lhs - rhs;
    return lhs;
}

constexpr
size&
operator*=(size& lhs, const size& rhs) noexcept
{
    lhs = lhs * rhs;
    return lhs;
}

constexpr
size&
operator/=(size& lhs, const size& rhs) noexcept
{
    lhs = lhs / rhs;
    return lhs;
}

// Arithmetic (binary, asymmetrical)

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
size
operator+(const size& lhs, T rhs) noexcept
{
    return size{lhs.w + rhs, lhs.h + rhs};
}

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
size
operator+(T lhs, const size& rhs) noexcept
{
    return rhs + lhs;
}

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
size
operator-(const size& lhs, T rhs) noexcept
{
    return size{lhs.w - rhs, lhs.h - rhs};
}

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
size
operator*(const size& lhs, T rhs) noexcept
{
    return size{lhs.w * rhs, lhs.h * rhs};
}

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
size
operator*(T lhs, const size& rhs) noexcept
{
    return rhs * lhs;
}

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
size
operator/(const size& lhs, T rhs) noexcept
{
    return size{lhs.w / rhs, lhs.h / rhs};
}

// Arithmetic (assignment, asymmetrical)

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
size&
operator+=(size& lhs, T rhs) noexcept
{
    lhs = lhs + rhs;
    return lhs;
}

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
size&
operator-=(size& lhs, T rhs) noexcept
{
    lhs = lhs - rhs;
    return lhs;
}

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
size&
operator*=(size& lhs, T rhs) noexcept
{
    lhs = lhs * rhs;
    return lhs;
}

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
size&
operator/=(size& lhs, T rhs) noexcept
{
    lhs = lhs / rhs;
    return lhs;
}

} // namespace sdlw::video
