#pragma once

#include <optional>
#include <tuple>

#include <SDL2/SDL_rect.h>

#include <sdlw/types.hpp>

namespace sdlw {

struct point : SDL_Point {};

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

struct rectangle : SDL_Rect {};

constexpr
bool
operator==(const rectangle& lhs, const rectangle& rhs) noexcept
{
    using std::tie;
    return tie(lhs.x, lhs.y, lhs.w, lhs.h) == tie(rhs.x, rhs.y, rhs.w, rhs.h);
}

constexpr
bool
operator!=(const rectangle& lhs, const rectangle& rhs) noexcept
{
    return !(lhs == rhs);
}

constexpr
size
size_of(const rectangle& rect) noexcept
{
    return size{rect.w, rect.h};
}

constexpr
point
position_of(const rectangle& rect) noexcept
{
    return point{rect.x, rect.y};
}

inline
std::optional<rectangle>
enclose_points(span<const point> points, const rectangle* clip) noexcept
{
    const auto sz = static_cast<int>(points.size());
    auto result = rectangle();
    if (SDL_EnclosePoints(points.data(), sz, clip, &result)) {
        return result;
    } else {
        return std::nullopt;
    }
}

inline
bool
are_intersecting(const rectangle& r1, const rectangle& r2) noexcept
{
    return SDL_HasIntersection(&r1, &r2);
}

inline
std::optional<rectangle>
intersection(const rectangle& r1, const rectangle& r2) noexcept
{
    if (auto result = rectangle(); SDL_IntersectRect(&r1, &r2, &result)) {
        return result;
    } else {
        return std::nullopt;
    }
}

inline
std::optional<std::pair<point, point>>
intersection(const rectangle& rect, point p1, point p2) noexcept
{
    if (SDL_IntersectRectAndLine(&rect, &p1.x, &p1.y, &p2.x, &p2.y)) {
        return std::make_pair(p1, p2);
    } else {
        return std::nullopt;
    }
}

inline
bool
is_point_in_rectangle(const point& p, const rectangle& rect) noexcept
{
    return SDL_PointInRect(&p, &rect);
}

inline
bool
is_empty(const rectangle& rect) noexcept
{
    return SDL_RectEmpty(&rect);
}

inline
rectangle
rectangle_union(const rectangle& r1, const rectangle& r2) noexcept
{
    auto result = rectangle();
    SDL_UnionRect(&r1, &r2, &result);
    return result;
}

} // namespace sdlw
