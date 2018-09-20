#pragma once

#include <algorithm>
#include <functional>
#include <unordered_map>

#include <sdlw/events/event.hpp>

namespace sdlw::events {

inline
void
pump_events() noexcept
{
    SDL_PumpEvents();
}

namespace filter::by_type {

    inline
    void
    set(event_type etype, bool enable) noexcept
    {
        const auto type = static_cast<u32>(etype);
        const auto state = static_cast<SDL_bool>(!enable);
        SDL_EventState(type, state);
    }

    inline
    bool
    is_enabled(event_type etype) noexcept
    {
        const auto type = static_cast<u32>(etype);
        return SDL_GetEventState(type) == SDL_ENABLE ? false : true;
    }

} // namespace filter::by_type

using event_filter = bool(const event&);

namespace filter::custom {

    namespace detail {

        inline std::function<event_filter> current_filter;

        inline
        int
        sdl_event_filter_callback(void*, SDL_Event* e) noexcept
        {
            if (current_filter) {
                auto p_event = reinterpret_cast<event*>(e);
                return static_cast<int>(current_filter(*p_event));
            } else {
                return static_cast<int>(true);
            }
        }

    } // namespace detail

    inline
    void
    set(std::function<event_filter> callback) noexcept
    {
        using namespace detail;
        SDL_SetEventFilter(nullptr, nullptr);
        current_filter = std::move(callback);
        SDL_SetEventFilter(sdl_event_filter_callback, nullptr);
    }

    inline
    const std::function<event_filter>&
    get() noexcept
    {
        return detail::current_filter;
    }

} // namespace filter::custom

using event_watch = void(const event&);

namespace watch {

    namespace detail {

        using map = std::unordered_map<int, std::function<event_watch>>;

        inline map event_watch_callbacks;

        inline
        int
        sdl_event_watch_callback(void*, SDL_Event* e) noexcept
        {
            for (const auto& [_, callback] : event_watch_callbacks) {
                if (callback) {
                    const auto p_event = reinterpret_cast<event*>(e);
                    callback(*p_event);
                }
            }
            return 0;
        }

    } // namespace detail

    inline
    void
    add(int id, std::function<event_watch> callback) noexcept
    {
        using namespace detail;
        if (callback) {
            SDL_DelEventWatch(sdl_event_watch_callback, nullptr);
            event_watch_callbacks[id] = std::move(callback);
            SDL_AddEventWatch(sdl_event_watch_callback, nullptr);
        }
    }

    inline
    void
    remove(int id) noexcept
    {
        using namespace detail;
        SDL_DelEventWatch(sdl_event_watch_callback, nullptr);
        event_watch_callbacks.erase(id);
        SDL_AddEventWatch(sdl_event_watch_callback, nullptr);
    }

} // namespace watch

namespace event_queue {

    inline
    void
    flush(event_type etype) noexcept
    {
        SDL_FlushEvent(static_cast<u32>(etype));
    }

    inline
    void
    flush(event_type min, event_type max)
    {
        SDL_FlushEvents(static_cast<u32>(min), static_cast<u32>(max));
    }

    namespace detail {

        inline
        int
        sdl_event_filter_callback(void* userdata, SDL_Event* e) noexcept
        {
            using function_type = std::function<event_filter>;
            const auto& filter = *reinterpret_cast<function_type*>(userdata);
            if (filter) {
                const auto p_event = reinterpret_cast<event*>(e);
                return static_cast<int>(filter(*p_event));
            } else {
                return static_cast<int>(true);
            }
        }

    } // namespace detail

    inline
    void
    filter(const std::function<event_filter>& f) noexcept
    {
        const auto p_filter = const_cast<std::function<event_filter>*>(&f);
        SDL_FilterEvents(detail::sdl_event_filter_callback, p_filter);
    }

    inline
    bool
    has(event_type etype) noexcept
    {
        return SDL_HasEvent(static_cast<u32>(etype));
    }

    inline
    bool
    has(event_type min, event_type max) noexcept
    {
        const auto min_ = static_cast<u32>(min);
        const auto max_ = static_cast<u32>(max);
        return SDL_HasEvents(min_, max_);
    }

    inline
    bool
    was_quit_requested() noexcept
    {
        return SDL_QuitRequested();
    }

    inline
    bool
    push(const event& e)
    {
        const auto p_event = const_cast<event*>(&e);
        const auto p_sdl_event = reinterpret_cast<SDL_Event*>(p_event);
        const auto result = SDL_PushEvent(p_sdl_event);
        if (result < 0) {
            throw error();
        } else {
            return static_cast<bool>(result);
        }
    }

    inline
    bool
    poll(event& e) noexcept
    {
        return SDL_PollEvent(reinterpret_cast<SDL_Event*>(&e));
    }

    inline
    int
    add(span<const event> events, event_type min, event_type max)
    {
        const auto cevents = const_cast<event*>(events.data());
        const auto p_events = reinterpret_cast<SDL_Event*>(cevents);
        const auto size = static_cast<int>(events.size());
        const auto add = SDL_ADDEVENT;
        const auto amin = static_cast<u32>(min);
        const auto amax = static_cast<u32>(max);
        const auto res = SDL_PeepEvents(p_events, size, add, amin, amax);
        if (res < 0) {
            throw error();
        } else {
            return res;
        }
    }

    inline
    int
    peek(span<event> events, event_type min, event_type max)
    {
        const auto cevents = const_cast<event*>(events.data());
        const auto p_events = reinterpret_cast<SDL_Event*>(cevents);
        const auto size = static_cast<int>(events.size());
        const auto peek = SDL_PEEKEVENT;
        const auto amin = static_cast<u32>(min);
        const auto amax = static_cast<u32>(max);
        const auto res = SDL_PeepEvents(p_events, size, peek, amin, amax);
        if (res < 0) {
            throw error();
        } else {
            return res;
        }
    }

    inline
    int
    get(span<event> events, event_type min, event_type max)
    {
        const auto cevents = const_cast<event*>(events.data());
        const auto p_events = reinterpret_cast<SDL_Event*>(cevents);
        const auto size = static_cast<int>(events.size());
        const auto get = SDL_GETEVENT;
        const auto amin = static_cast<u32>(min);
        const auto amax = static_cast<u32>(max);
        const auto res = SDL_PeepEvents(p_events, size, get, amin, amax);
        if (res < 0) {
            throw error();
        } else {
            return res;
        }
    }

    inline
    bool
    await(event& e) noexcept
    {
        return SDL_WaitEvent(reinterpret_cast<SDL_Event*>(&e)) == 1;
    }

    template<typename Rep, typename Period>
    inline
    bool
    await_for(event& e, const std::chrono::duration<Rep, Period>& timeout)
    noexcept
    {
        using namespace std::chrono;
        const auto pevent = reinterpret_cast<SDL_Event*>(&e);
        const auto timeout_ = duration_cast<time::clock::duration>(timeout);
        return SDL_WaitEventTimeout(pevent, timeout_.count()) == 1;
    }

} // namespace queue

} // namespace sdlw::events
