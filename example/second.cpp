#include <sdlw/rectangle.hpp>

void foo() {
    auto r1 = sdlw::rectangle{};
    auto r2 = sdlw::rectangle{};
    r1 == r2;

    auto p1 = sdlw::point{};
    auto p2 = sdlw::point{};
    p1 == p2;
}
