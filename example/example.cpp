#include <sdlw/sdlw.hpp>

using namespace std;
using namespace std::chrono;
using namespace sdlw;
using namespace sdlw::events;
using namespace sdlw::time;
using namespace sdlw::video;

static auto points = std::vector<point>();

void handle_event(const event& e)
{
    using namespace sdlw::events::mouse;
    if (e.type() == event_type::mouse_motion) {
        const auto state = e.mouse_motion().state();
        const auto pressed = state & button_mask(button::left);
        if (static_cast<bool>(pressed)) {
            const auto x = e.mouse_motion().x();
            const auto y = e.mouse_motion().y();
            points.push_back(point{x, y});
        }
    }
}

void run()
{
    const auto sdlw_guard = sdlw::subsystem(sdlw::subsystem_flags::video);
    const auto win_size = size_of(display::usable_bounds(0)) / 2;
    const auto win_bounds = rectangle{50, 50, win_size.w, win_size.h};
    auto win = window("title", win_bounds, window_flags::shown);
    get_window(win.id()).set_title("new title");
    auto rend = renderer(win, renderer_flags::accelerated);
    auto e = event();
    for (;;) {
        while (event_queue::poll(e)) {
            if (e.type() == event_type::quit) {
                return;
            } else {
                handle_event(e);
            }
        }
        rend.set_draw_color(color{0, 0, 0});
        rend.clear();
        rend.set_draw_color(color{255, 0, 0});
        rend.draw_line_strip(points);
        rend.present();
    }
}

int
main(int argc, char* argv[])
{
    try {
        run();
    } catch (const sdlw::error& e) {
        SDL_Log("sdlw error: %s\n", e.what());
    } catch (...) {
        SDL_Log("unknown exception\n");
    }
    return 0;
}
