#include <sdlw/sdlw.hpp>
#include <sdlw/image.hpp>
#include <sdlw/ttf.hpp>

using namespace std;
using namespace std::chrono;
using namespace sdlw;
using namespace sdlw::events;
using namespace sdlw::time;
using namespace sdlw::video;

static auto points = std::vector<point>();

void handle_event(const event &e) {
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

void run() {
    // Initialize the subsystems
    const auto sdlw_guard = sdlw::subsystem(sdlw::subsystem_flags::video);
    // Create the window
    const auto win_size = size_of(display::usable_bounds(0)) / 2;
    const auto win_bounds = rectangle{50, 50, win_size.w, win_size.h};
    auto win = window("title", win_bounds, window_flags::shown);
    get_window(win.id()).set_title("new title");
    // Create the renderer
    auto rend = renderer(win, renderer_flags::accelerated);
    auto e = event();
    // Watch events
    constexpr auto custom_watch = [] (const event &e, void *) {
        if (e.type() == event_type::mouse_button_down) {
            SDL_Log("mouse button down!\n");
        }
    };
    watch::add<custom_watch>();
    for (;;) {
        // Handle the events
        while (event_queue::poll(e)) {
            if (e.type() == event_type::quit) return;
            else handle_event(e);
        }
        // Do the rendering
        rend.set_draw_color(color{0, 0, 0});
        rend.clear();
        rend.set_draw_color(color{255, 0, 0});
        rend.draw_line_strip(points);
        rend.present();
    }
}

auto main(int argc, char *argv[]) -> int {
    try {
        run();
    } catch (const sdlw::error &e) {
        SDL_Log("sdlw error: %s\n", e.what());
    } catch (...) {
        SDL_Log("unknown exception\n");
    }
    return 0;
}
