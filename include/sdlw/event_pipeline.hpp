#pragma once

#include <sdlw/event.hpp>

namespace sdlw {

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

namespace filter::custom {

    template<typename EventFilter>
    void set(EventFilter& f) {
        static_assert(std::is_invocable_r_v<bool, EventFilter, const event&>);
        constexpr auto sdl_callback = [] (void* userdata, SDL_Event* e) -> int {
            auto& filter = *static_cast<EventFilter*>(userdata);
            const auto& ev = *reinterpret_cast<event*>(e);
            return static_cast<int>(filter(ev));
        };
        SDL_SetEventFilter(sdl_callback, &f);
    }

    void set(bool(*filter)(const event&)) {
        constexpr auto fp_sdl_callback = [] (void* userdata, SDL_Event* e) -> int {
            auto f = reinterpret_cast<bool(*)(const event&)>(userdata);
            const auto& ev = *reinterpret_cast<event*>(e);
            return static_cast<int>(f(ev));
        };
        SDL_SetEventFilter(fp_sdl_callback, reinterpret_cast<void*>(filter));
    }

} // namespace filter::custom

namespace watch {

    namespace detail {

        template<typename EventWatch>
        struct watch_generator {
            static auto sdl_callback(void* userdata, SDL_Event* e) -> int {
                auto& watch = *static_cast<EventWatch*>(userdata);
                const auto& ev = *reinterpret_cast<event*>(e);
                watch(ev);
                return 0; // return value is ignored by SDL2
            }
        };

        inline auto fp_sdl_callback(void* userdata, SDL_Event* e) -> int {
            auto watch = reinterpret_cast<void(*)(const event&)>(userdata);
            const auto& ev = *reinterpret_cast<event*>(e);
            watch(ev);
            return 0; // return value is ignored by SDL2
        }

    } // namespace detail

    template<typename EventWatch>
    void add(EventWatch& w) {
        static_assert(std::is_invocable_r_v<void, EventWatch, const event&>);
        SDL_AddEventWatch(detail::watch_generator<EventWatch>::sdl_callback, &w);
    }

    void add(void(*watch)(const event&)) {
        SDL_AddEventWatch(detail::fp_sdl_callback, reinterpret_cast<void*>(watch));
    }

    template<typename EventWatch>
    void remove(EventWatch& w) {
        static_assert(std::is_invocable_r_v<void, EventWatch, const event&>);
        SDL_DelEventWatch(detail::watch_generator<EventWatch>::sdl_callback, &w);
    }

    void remove(void(*watch)(const event&)) {
        SDL_DelEventWatch(detail::fp_sdl_callback, reinterpret_cast<void*>(watch));
    }

} // namespace watch

namespace event_queue {

    inline void flush(event_type etype) noexcept {
        SDL_FlushEvent(static_cast<u32>(etype));
    }

    inline void flush(event_type min, event_type max) {
        SDL_FlushEvents(static_cast<u32>(min), static_cast<u32>(max));
    }

    template<typename EventFilter>
    void filter(EventFilter& f) {
        static_assert(std::is_invocable_r_v<bool, EventFilter, const event&>);
        constexpr auto sdl_callback = [] (void* userdata, SDL_Event* e) -> int {
            auto& filter = *static_cast<EventFilter*>(userdata);
            const auto& ev = *reinterpret_cast<event*>(e);
            return static_cast<bool>(filter(ev));
        };
        SDL_FilterEvents(sdl_callback, &f);
    }

    void filter(bool(*filter)(const event&)) {
        constexpr auto fp_sdl_callback = [] (void* userdata, SDL_Event* e) -> int {
            auto f = reinterpret_cast<bool(*)(const event&)>(userdata);
            const auto& ev = *reinterpret_cast<event*>(e);
            return static_cast<bool>(f(ev));
        };
        SDL_FilterEvents(fp_sdl_callback, reinterpret_cast<void*>(filter));
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
        return SDL_PollEvent(reinterpret_cast<SDL_Event*>(&e));
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

} // namespace sdlw
