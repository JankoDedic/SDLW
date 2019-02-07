#pragma once

#include <sdlw/events/event.hpp>

namespace sdlw::events {

inline void pump_events() noexcept {
    SDL_PumpEvents();
}

namespace filter::by_type {

    inline void set(event_type etype, bool enable) noexcept {
        const auto type = static_cast<u32>(etype);
        const auto state = static_cast<SDL_bool>(!enable);
        SDL_EventState(type, state);
    }

    inline auto is_enabled(event_type etype) noexcept -> bool {
        const auto type = static_cast<u32>(etype);
        return SDL_GetEventState(type) == SDL_ENABLE ? false : true;
    }

} // namespace filter::by_type

using event_filter = bool(const event &, void *);

namespace filter::custom {

    template<event_filter EventFilter>
    inline void set(void *userdata = nullptr) noexcept {
        constexpr auto callback = [] (void *userdata, SDL_Event *e) -> int {
            return static_cast<int>(EventFilter(*reinterpret_cast<event *>(e), userdata));
        };
        SDL_SetEventFilter(callback, userdata);
    }

} // namespace filter::custom

using event_watch = void(const event &, void *);

namespace watch {

    namespace detail {

        template<event_watch EventWatch>
        struct watch_generator {
            static auto callback(void *userdata, SDL_Event *e) -> int {
                EventWatch(*reinterpret_cast<event *>(e), userdata);
                return 0; // return value is ignored by SDL
            }
        };

    } // namespace detail

    template<event_watch EventWatch>
    inline void add(void *userdata = nullptr) noexcept {
        SDL_AddEventWatch(detail::watch_generator<EventWatch>::callback, userdata);
    }

    template<event_watch EventWatch>
    inline void remove(void *userdata = nullptr) noexcept {
        SDL_DelEventWatch(detail::watch_generator<EventWatch>::callback, userdata);
    }

} // namespace watch

namespace event_queue {

    inline void flush(event_type etype) noexcept {
        SDL_FlushEvent(static_cast<u32>(etype));
    }

    inline void flush(event_type min, event_type max) {
        SDL_FlushEvents(static_cast<u32>(min), static_cast<u32>(max));
    }

    template<event_filter EventFilter>
    inline void filter(void *userdata = nullptr) noexcept {
        constexpr auto callback = [] (void *data, SDL_Event *e) -> int {
            return static_cast<int>(EventFilter(reinterpret_cast<event *>(e), data));
        };
        SDL_FilterEvents(callback, userdata);
    }

    inline auto has(event_type etype) noexcept -> bool {
        return SDL_HasEvent(static_cast<u32>(etype));
    }

    inline auto has(event_type min, event_type max) noexcept -> bool {
        return SDL_HasEvents(static_cast<u32>(min), static_cast<u32>(max));
    }

    inline auto was_quit_requested() noexcept -> bool {
        return SDL_QuitRequested();
    }

    inline auto push(const event &e) -> bool {
        const auto result = SDL_PushEvent(reinterpret_cast<SDL_Event *>(const_cast<event *>(&e)));
        if (result < 0) {
            throw error();
        } else {
            return static_cast<bool>(result);
        }
    }

    inline auto poll(event &e) noexcept -> bool {
        return SDL_PollEvent(reinterpret_cast<SDL_Event *>(&e));
    }

    inline auto add(span<const event> events, event_type min, event_type max) -> int {
        const auto cevents = const_cast<event *>(events.data());
        const auto p_events = reinterpret_cast<SDL_Event *>(cevents);
        const auto size = static_cast<int>(events.size());
        const auto amin = static_cast<u32>(min);
        const auto amax = static_cast<u32>(max);
        const auto res = SDL_PeepEvents(p_events, size, SDL_ADDEVENT, amin, amax);
        if (res < 0) {
            throw error();
        } else {
            return res;
        }
    }

    inline auto peek(span<event> events, event_type min, event_type max) -> int {
        const auto cevents = const_cast<event *>(events.data());
        const auto p_events = reinterpret_cast<SDL_Event *>(cevents);
        const auto size = static_cast<int>(events.size());
        const auto amin = static_cast<u32>(min);
        const auto amax = static_cast<u32>(max);
        const auto res = SDL_PeepEvents(p_events, size, SDL_PEEKEVENT, amin, amax);
        if (res < 0) {
            throw error();
        } else {
            return res;
        }
    }

    inline auto get(span<event> events, event_type min, event_type max) -> int {
        const auto cevents = const_cast<event *>(events.data());
        const auto p_events = reinterpret_cast<SDL_Event *>(cevents);
        const auto size = static_cast<int>(events.size());
        const auto amin = static_cast<u32>(min);
        const auto amax = static_cast<u32>(max);
        const auto res = SDL_PeepEvents(p_events, size, SDL_GETEVENT, amin, amax);
        if (res < 0) {
            throw error();
        } else {
            return res;
        }
    }

    inline auto await(event &e) noexcept -> bool {
        return SDL_WaitEvent(reinterpret_cast<SDL_Event *>(&e)) == 1;
    }

    template<typename Rep, typename Period>
    inline auto await_for(event &e, const std::chrono::duration<Rep, Period> &timeout) noexcept -> bool {
        using namespace std::chrono;
        const auto pevent = reinterpret_cast<SDL_Event *>(&e);
        const auto timeout_ = duration_cast<time::clock::duration>(timeout);
        return SDL_WaitEventTimeout(pevent, timeout_.count()) == 1;
    }

} // namespace queue

} // namespace sdlw::events
