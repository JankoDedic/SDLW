#pragma once

#include <type_traits>

#include <SDL2/SDL.h>

namespace sdlw {

using point = SDL_Point;

// Comparison

constexpr
bool
operator==(const point& lhs, const point& rhs) noexcept
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

constexpr
bool
operator!=(const point& lhs, const point& rhs) noexcept
{
    return !(lhs == rhs);
}

constexpr
bool
operator<(const point& lhs, const point& rhs) noexcept
{
    return lhs.x < rhs.x && lhs.y < rhs.y;
}

constexpr
bool
operator>(const point& lhs, const point& rhs) noexcept
{
    return rhs < lhs;
}

constexpr
bool
operator<=(const point& lhs, const point& rhs) noexcept
{
    return !(rhs < lhs);
}

constexpr
bool
operator>=(const point& lhs, const point& rhs) noexcept
{
    return !(lhs < rhs);
}

// Arithmetic (unary)

constexpr
point
operator-(const point& p) noexcept
{
    return point{-p.x, -p.y};
}

// Arithmetic (binary, symmetrical)

constexpr
point
operator+(const point& lhs, const point& rhs) noexcept
{
    return point{lhs.x + rhs.x, lhs.y + rhs.y};
}

constexpr
point
operator-(const point& lhs, const point& rhs) noexcept
{
    return point{lhs.x - rhs.x, lhs.y - rhs.y};
}

constexpr
point
operator*(const point& lhs, const point& rhs) noexcept
{
    return point{lhs.x * rhs.x, lhs.y * rhs.y};
}

constexpr
point
operator/(const point& lhs, const point& rhs) noexcept
{
    return point{lhs.x / rhs.x, lhs.y / rhs.y};
}

// Arithmetic (assignment, symmetrical)

constexpr
point&
operator+=(point& lhs, const point& rhs) noexcept
{
    lhs = lhs + rhs;
    return lhs;
}

constexpr
point&
operator-=(point& lhs, const point& rhs) noexcept
{
    lhs = lhs - rhs;
    return lhs;
}

constexpr
point&
operator*=(point& lhs, const point& rhs) noexcept
{
    lhs = lhs * rhs;
    return lhs;
}

constexpr
point&
operator/=(point& lhs, const point& rhs) noexcept
{
    lhs = lhs / rhs;
    return lhs;
}

// Arithmetic (binary, asymmetrical)

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
point
operator+(const point& lhs, T rhs) noexcept
{
    return point{lhs.x + rhs, lhs.y + rhs};
}

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
point
operator+(T lhs, const point& rhs) noexcept
{
    return rhs + lhs;
}

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
point
operator-(const point& lhs, T rhs) noexcept
{
    return point{lhs.x - rhs, lhs.y - rhs};
}

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
point
operator*(const point& lhs, T rhs) noexcept
{
    return point{lhs.x * rhs, lhs.y * rhs};
}

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
point
operator*(T lhs, const point& rhs) noexcept
{
    return rhs * lhs;
}

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
point
operator/(const point& lhs, T rhs) noexcept
{
    return point{lhs.x / rhs, lhs.y / rhs};
}

// Arithmetic (assignment, asymmetrical)

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
point&
operator+=(point& lhs, T rhs) noexcept
{
    lhs = lhs + rhs;
    return lhs;
}

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
point&
operator-=(point& lhs, T rhs) noexcept
{
    lhs = lhs - rhs;
    return lhs;
}

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
point&
operator*=(point& lhs, T rhs) noexcept
{
    lhs = lhs * rhs;
    return lhs;
}

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr
point&
operator/=(point& lhs, T rhs) noexcept
{
    lhs = lhs / rhs;
    return lhs;
}

} // namespace sdlw
