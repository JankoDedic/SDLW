#include <sdlw/rect.hpp>

void foo() {
    auto r1 = sdlw::rect{};
    auto r2 = sdlw::rect{};
    r1 == r2;

    auto p1 = sdlw::point{};
    auto p2 = sdlw::point{};
    p1 == p2;
}
