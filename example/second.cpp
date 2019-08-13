#include <sdlw/rect.hpp>

void foo() {
    auto r1 = sdl::rect{};
    auto r2 = sdl::rect{};
    r1 == r2;

    auto p1 = sdl::point{};
    auto p2 = sdl::point{};
    p1 == p2;
}
