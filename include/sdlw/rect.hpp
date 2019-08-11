#pragma once

#include <optional>
#include <tuple>

#include <SDL2/SDL_rect.h>

#include <sdlw/types.hpp>

namespace sdlw {

// clang-format off

struct point : SDL_Point {
    point() = default;

    constexpr point(const SDL_Point& p) noexcept
        : SDL_Point{p}
    {
    }

    constexpr point(int x, int y) noexcept
        : SDL_Point{x, y}
    {
    }
};

// Comparison

constexpr auto operator== (const point& lhs, const point& rhs) noexcept -> bool { return lhs.x == rhs.x && lhs.y == rhs.y; }
constexpr auto operator!= (const point& lhs, const point& rhs) noexcept -> bool { return !(lhs == rhs);                    }
constexpr auto operator<  (const point& lhs, const point& rhs) noexcept -> bool { return lhs.x < rhs.x && lhs.y < rhs.y;   }
constexpr auto operator>  (const point& lhs, const point& rhs) noexcept -> bool { return rhs < lhs;                        }
constexpr auto operator<= (const point& lhs, const point& rhs) noexcept -> bool { return !(rhs < lhs);                     }
constexpr auto operator>= (const point& lhs, const point& rhs) noexcept -> bool { return !(lhs < rhs);                     }

// Arithmetic (unary)

constexpr auto operator-(const point& p) noexcept -> point { return {-p.x, -p.y}; }

// Arithmetic (binary, symmetrical)

constexpr auto operator+(const point& lhs, const point& rhs) noexcept -> point { return point{lhs.x + rhs.x, lhs.y + rhs.y}; }
constexpr auto operator-(const point& lhs, const point& rhs) noexcept -> point { return point{lhs.x - rhs.x, lhs.y - rhs.y}; }
constexpr auto operator*(const point& lhs, const point& rhs) noexcept -> point { return point{lhs.x * rhs.x, lhs.y * rhs.y}; }
constexpr auto operator/(const point& lhs, const point& rhs) noexcept -> point { return point{lhs.x / rhs.x, lhs.y / rhs.y}; }

// Arithmetic (assignment, symmetrical)

constexpr auto operator+=(point& lhs, const point& rhs) noexcept -> point& { return lhs = lhs + rhs; }
constexpr auto operator-=(point& lhs, const point& rhs) noexcept -> point& { return lhs = lhs - rhs; }
constexpr auto operator*=(point& lhs, const point& rhs) noexcept -> point& { return lhs = lhs * rhs; }
constexpr auto operator/=(point& lhs, const point& rhs) noexcept -> point& { return lhs = lhs / rhs; }

// Arithmetic (binary, asymmetrical)

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator+(const point& lhs, T rhs) noexcept -> point { return {lhs.x + rhs, lhs.y + rhs}; }

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator+(T lhs, const point& rhs) noexcept -> point { return rhs + lhs; }

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator-(const point& lhs, T rhs) noexcept -> point { return {lhs.x - rhs, lhs.y - rhs}; }

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator*(const point& lhs, T rhs) noexcept -> point { return {lhs.x * rhs, lhs.y * rhs}; }

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator*(T lhs, const point& rhs) noexcept -> point { return rhs * lhs; }

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator/(const point& lhs, T rhs) noexcept -> point { return {lhs.x / rhs, lhs.y / rhs}; }

// Arithmetic (assignment, asymmetrical)

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator+=(point& lhs, T rhs) noexcept -> point& { return lhs = lhs + rhs; }

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator-=(point& lhs, T rhs) noexcept -> point& { return lhs = lhs - rhs; }

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator*=(point& lhs, T rhs) noexcept -> point& { return lhs = lhs * rhs; }

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator/=(point& lhs, T rhs) noexcept -> point& { return lhs = lhs / rhs; }

struct size {
    int w;
    int h;
};

// Comparison

constexpr auto operator== (const size& lhs, const size& rhs) noexcept -> bool { return lhs.w == rhs.w && lhs.h == rhs.h; }
constexpr auto operator!= (const size& lhs, const size& rhs) noexcept -> bool { return !(lhs == rhs);                    }
constexpr auto operator<  (const size& lhs, const size& rhs) noexcept -> bool { return lhs.w < rhs.w && lhs.h < rhs.h;   }
constexpr auto operator>  (const size& lhs, const size& rhs) noexcept -> bool { return rhs < lhs;                        }
constexpr auto operator<= (const size& lhs, const size& rhs) noexcept -> bool { return !(rhs < lhs);                     }
constexpr auto operator>= (const size& lhs, const size& rhs) noexcept -> bool { return !(lhs < rhs);                     }

// Arithmetic (unary)

constexpr auto operator-(const size& s) noexcept -> size { return {-s.w, -s.h}; }

// Arithmetic (binary, symmetrical)

constexpr auto operator+(const size& lhs, const size& rhs) noexcept -> size { return {lhs.w + rhs.w, lhs.h + rhs.h}; }
constexpr auto operator-(const size& lhs, const size& rhs) noexcept -> size { return {lhs.w - rhs.w, lhs.h - rhs.h}; }
constexpr auto operator*(const size& lhs, const size& rhs) noexcept -> size { return {lhs.w * rhs.w, lhs.h * rhs.h}; }
constexpr auto operator/(const size& lhs, const size& rhs) noexcept -> size { return {lhs.w / rhs.w, lhs.h / rhs.h}; }

// Arithmetic (assingment, symmetrical)

constexpr auto operator+=(size& lhs, const size& rhs) noexcept -> size& { return lhs = lhs + rhs; }
constexpr auto operator-=(size& lhs, const size& rhs) noexcept -> size& { return lhs = lhs - rhs; }
constexpr auto operator*=(size& lhs, const size& rhs) noexcept -> size& { return lhs = lhs * rhs; }
constexpr auto operator/=(size& lhs, const size& rhs) noexcept -> size& { return lhs = lhs / rhs; }

// Arithmetic (binary, asymmetrical)

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator+(const size& lhs, T rhs) noexcept -> size { return {lhs.w + rhs, lhs.h + rhs}; }

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator+(T lhs, const size& rhs) noexcept -> size { return rhs + lhs; }

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator-(const size& lhs, T rhs) noexcept -> size { return {lhs.w - rhs, lhs.h - rhs}; }

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator*(const size& lhs, T rhs) noexcept -> size { return {lhs.w * rhs, lhs.h * rhs}; }

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator*(T lhs, const size& rhs) noexcept -> size { return rhs * lhs; }

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator/(const size& lhs, T rhs) noexcept -> size { return {lhs.w / rhs, lhs.h / rhs}; }

// Arithmetic (assignment, asymmetrical)

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator+=(size& lhs, T rhs) noexcept -> size& { return lhs = lhs + rhs; }

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator-=(size& lhs, T rhs) noexcept -> size& { return lhs = lhs - rhs; }

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator*=(size& lhs, T rhs) noexcept -> size& { return lhs = lhs * rhs; }

template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator/=(size& lhs, T rhs) noexcept -> size& { return lhs = lhs / rhs; }

struct rect : SDL_Rect {
    rect() = default;

    constexpr rect(const SDL_Rect& r) noexcept
        : SDL_Rect{r}
    {
    }

    constexpr rect(int x, int y, int w, int h) noexcept
        : SDL_Rect{x, y, w, h}
    {
    }
};

constexpr auto operator==(const rect& lhs, const rect& rhs) noexcept -> bool {
    return lhs.x == rhs.x
        && lhs.y == rhs.y
        && lhs.w == rhs.w
        && lhs.h == rhs.h;
}

constexpr auto operator!=(const rect& lhs, const rect& rhs) noexcept -> bool {
    return !(lhs == rhs);
}

// clang-format on

constexpr auto size_of(const rect& rect) noexcept -> size
{
    return {rect.w, rect.h};
}

constexpr auto position_of(const rect& rect) noexcept -> point
{
    return {rect.x, rect.y};
}

inline auto enclose_points(span<const point> points, const rect* clip) noexcept -> std::optional<rect>
{
    const auto sz = static_cast<int>(points.size());
    auto result = rect{};
    if (SDL_EnclosePoints(points.data(), sz, clip, &result)) {
        return result;
    } else {
        return std::nullopt;
    }
}

inline auto are_intersecting(const rect& r1, const rect& r2) noexcept -> bool
{
    return SDL_HasIntersection(&r1, &r2);
}

inline auto intersection(const rect& r1, const rect& r2) noexcept -> std::optional<rect>
{
    if (auto result = rect{}; SDL_IntersectRect(&r1, &r2, &result)) {
        return result;
    } else {
        return std::nullopt;
    }
}

inline auto intersection(const rect& rect, point p1, point p2) noexcept -> std::optional<std::pair<point, point>>
{
    if (SDL_IntersectRectAndLine(&rect, &p1.x, &p1.y, &p2.x, &p2.y)) {
        return std::make_pair(p1, p2);
    } else {
        return std::nullopt;
    }
}

inline auto is_point_in_rectangle(const point& p, const rect& rect) noexcept -> bool
{
    return SDL_PointInRect(&p, &rect);
}

inline auto is_empty(const rect& rect) noexcept -> bool
{
    return SDL_RectEmpty(&rect);
}

inline auto rectangle_union(const rect& r1, const rect& r2) noexcept -> rect
{
    auto result = rect{};
    SDL_UnionRect(&r1, &r2, &result);
    return result;
}

} // namespace sdlw
