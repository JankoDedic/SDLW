#pragma once

#include <tuple>

#include <SDL2/SDL.h>

#include <sdlw/point.hpp>
#include <sdlw/size.hpp>

namespace sdlw {

using rectangle = SDL_Rect;

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
