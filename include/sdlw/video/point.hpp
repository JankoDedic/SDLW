#pragma once

#include <SDL2/SDL.h>

namespace sdlw::video {

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

constexpr
point
operator+(const point& lhs, int rhs) noexcept
{
    return point{lhs.x + rhs, lhs.y + rhs};
}

constexpr
point
operator+(int lhs, const point& rhs) noexcept
{
    return rhs + lhs;
}

constexpr
point
operator-(const point& lhs, int rhs) noexcept
{
    return point{lhs.x - rhs, lhs.y - rhs};
}

constexpr
point
operator*(const point& lhs, int rhs) noexcept
{
    return point{lhs.x * rhs, lhs.y * rhs};
}

constexpr
point
operator*(int lhs, const point& rhs) noexcept
{
    return rhs * lhs;
}

constexpr
point
operator/(const point& lhs, int rhs) noexcept
{
    return point{lhs.x / rhs, lhs.y / rhs};
}

// Arithmetic (assignment, asymmetrical)

constexpr
point&
operator+=(point& lhs, int rhs) noexcept
{
    lhs = lhs + rhs;
    return lhs;
}

constexpr
point&
operator-=(point& lhs, int rhs) noexcept
{
    lhs = lhs - rhs;
    return lhs;
}

constexpr
point&
operator*=(point& lhs, int rhs) noexcept
{
    lhs = lhs * rhs;
    return lhs;
}

constexpr
point&
operator/=(point& lhs, int rhs) noexcept
{
    lhs = lhs / rhs;
    return lhs;
}

} // namespace sdlw::video
