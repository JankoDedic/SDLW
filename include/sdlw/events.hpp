#pragma once

#include <optional>

#include <SDL2/SDL_events.h>

#include <sdlw/audio.hpp>
#include <sdlw/game_controller.hpp>
#include <sdlw/gesture.hpp>
#include <sdlw/joystick.hpp>
#include <sdlw/keyboard.hpp>
#include <sdlw/mouse.hpp>
#include <sdlw/timer.hpp>
#include <sdlw/touch.hpp>
#include <sdlw/video.hpp>

namespace sdl {

// clang-format off

enum class event_type {
    first_event                = SDL_FIRSTEVENT,
    quit                       = SDL_QUIT,
    app_terminating            = SDL_APP_TERMINATING,
    app_low_memory             = SDL_APP_LOWMEMORY,
    app_will_enter_background  = SDL_APP_WILLENTERBACKGROUND,
    app_did_enter_background   = SDL_APP_DIDENTERBACKGROUND,
    app_will_enter_foreground  = SDL_APP_WILLENTERFOREGROUND,
    app_did_enter_foreground   = SDL_APP_DIDENTERFOREGROUND,
    window                     = SDL_WINDOWEVENT,
    syswm                      = SDL_SYSWMEVENT,
    key_down                   = SDL_KEYDOWN,
    key_up                     = SDL_KEYUP,
    text_editing               = SDL_TEXTEDITING,
    text_input                 = SDL_TEXTINPUT,
    keymap_changed             = SDL_KEYMAPCHANGED,
    mouse_motion               = SDL_MOUSEMOTION,
    mouse_button_down          = SDL_MOUSEBUTTONDOWN,
    mouse_button_up            = SDL_MOUSEBUTTONUP,
    mouse_wheel                = SDL_MOUSEWHEEL,
    joy_axis_motion            = SDL_JOYAXISMOTION,
    joy_ball_motion            = SDL_JOYBALLMOTION,
    joy_hat_motion             = SDL_JOYHATMOTION,
    joy_button_down            = SDL_JOYBUTTONDOWN,
    joy_button_up              = SDL_JOYBUTTONUP,
    joy_device_added           = SDL_JOYDEVICEADDED,
    joy_device_removed         = SDL_JOYDEVICEREMOVED,
    controller_axis_motion     = SDL_CONTROLLERAXISMOTION,
    controller_button_down     = SDL_CONTROLLERBUTTONDOWN,
    controller_button_up       = SDL_CONTROLLERBUTTONUP,
    controller_device_added    = SDL_CONTROLLERDEVICEADDED,
    controller_device_removed  = SDL_CONTROLLERDEVICEREMOVED,
    controller_device_remapped = SDL_CONTROLLERDEVICEREMAPPED,
    finger_down                = SDL_FINGERDOWN,
    finger_up                  = SDL_FINGERUP,
    finger_motion              = SDL_FINGERMOTION,
    dollar_gesture             = SDL_DOLLARGESTURE,
    dollar_record              = SDL_DOLLARRECORD,
    multi_gesture              = SDL_MULTIGESTURE,
    clipboard_update           = SDL_CLIPBOARDUPDATE,
    drop_file                  = SDL_DROPFILE,
    drop_text                  = SDL_DROPTEXT,
    drop_begin                 = SDL_DROPBEGIN,
    drop_complete              = SDL_DROPCOMPLETE,
    audio_device_added         = SDL_AUDIODEVICEADDED,
    audio_device_removed       = SDL_AUDIODEVICEREMOVED,
    render_targets_reset       = SDL_RENDER_TARGETS_RESET,
    render_device_reset        = SDL_RENDER_DEVICE_RESET,
    user_event                 = SDL_USEREVENT,
    last_event                 = SDL_LASTEVENT
};

class common_event {
    SDL_CommonEvent _event;

public:
    auto type()      const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
};

class audio_device_event {
    SDL_AudioDeviceEvent _event;

public:
    auto type()       const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp()  const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto is_capture() const noexcept -> bool              { return static_cast<bool>(_event.iscapture);                  }
    auto which()      const noexcept -> u32               { return _event.which;                                         }
};

class controller_axis_event {
    SDL_ControllerAxisEvent _event;

public:
    auto type()      const noexcept -> event_type           { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point    { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto which()     const noexcept -> joystick_id          { return _event.which;                                         }
    auto axis()      const noexcept -> game_controller_axis { return static_cast<game_controller_axis>(_event.axis);       }
    auto value()     const noexcept -> i16                  { return _event.value;                                         }
};

class controller_button_event {
    SDL_ControllerButtonEvent _event;

public:
    auto type()      const noexcept -> event_type             { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point      { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto which()     const noexcept -> joystick_id            { return _event.which;                                         }
    auto button()    const noexcept -> game_controller_button { return static_cast<game_controller_button>(_event.button);   }
};

class controller_device_event {
    SDL_ControllerDeviceEvent _event;

public:
    auto type()      const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto which()     const noexcept -> i32               { return _event.which;                                         }
};

class dollar_gesture_event {
    SDL_DollarGestureEvent _event;

public:
    auto type()        const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp()   const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto touch_id()    const noexcept -> sdl::touch_id     { return {_event.touchId};                                     }
    auto gesture_id()  const noexcept -> gesture_id        { return {_event.gestureId};                                   }
    auto num_fingers() const noexcept -> u32               { return _event.numFingers;                                    }
    auto error()       const noexcept -> float             { return _event.error;                                         }
    auto x()           const noexcept -> float             { return _event.x;                                             }
    auto y()           const noexcept -> float             { return _event.y;                                             }
};

class drop_event {
    SDL_DropEvent _event;

public:
    auto type()      const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto window_id() const noexcept -> window_id         { return {_event.windowID};                                    }
    auto file()      const noexcept -> const char*       { return _event.file;                                          }
};

class touch_finger_event {
    SDL_TouchFingerEvent _event;

public:
    auto type()      const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto touch_id()  const noexcept -> sdl::touch_id     { return {_event.touchId};                                     }
    auto finger_id() const noexcept -> sdl::finger_id    { return {_event.fingerId};                                    }
    auto x()         const noexcept -> float             { return _event.x;                                             }
    auto y()         const noexcept -> float             { return _event.y;                                             }
    auto dx()        const noexcept -> float             { return _event.dx;                                            }
    auto dy()        const noexcept -> float             { return _event.dy;                                            }
    auto pressure()  const noexcept -> float             { return _event.pressure;                                      }
};

class keyboard_event {
    SDL_KeyboardEvent _event;

public:
    auto type()      const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto window_id() const noexcept -> window_id         { return {_event.windowID};                                    }
    auto key()       const noexcept -> sdl::key          { return sdl::key{_event.keysym};                              }
    auto repeat()    const noexcept -> int               { return _event.repeat;                                        }
};

class joy_axis_event {
    SDL_JoyAxisEvent _event;

public:
    auto type()      const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto which()     const noexcept -> joystick_id       { return {_event.which};                                       }
    auto axis()      const noexcept -> u8                { return _event.axis;                                          }
    auto value()     const noexcept -> i16               { return _event.value;                                         }
};

class joy_ball_event {
    SDL_JoyBallEvent _event;

public:
    auto type()      const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto which()     const noexcept -> joystick_id       { return {_event.which};                                       }
    auto ball()      const noexcept -> u8                { return _event.ball;                                          }
    auto xrel()      const noexcept -> i16               { return _event.xrel;                                          }
    auto yrel()      const noexcept -> i16               { return _event.yrel;                                          }
};

class joy_hat_event {
    SDL_JoyHatEvent _event;

public:
    auto type()      const noexcept -> event_type            { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point     { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto which()     const noexcept -> joystick_id           { return {_event.which};                                       }
    auto hat()       const noexcept -> u8                    { return _event.hat;                                           }
    auto value()     const noexcept -> joystick_hat_position { return static_cast<joystick_hat_position>(_event.value);     }
};

class joy_button_event {
    SDL_JoyButtonEvent _event;

public:
    auto type()      const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto which()     const noexcept -> joystick_id       { return {_event.which};                                       }
    auto button()    const noexcept -> u8                { return _event.button;                                        }
};

class joy_device_event {
    SDL_JoyDeviceEvent _event;

public:
    auto type()      const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto which()     const noexcept -> i32               { return _event.which;                                         }
};

class mouse_motion_event {
    SDL_MouseMotionEvent _event;

public:
    auto type()      const noexcept -> event_type         { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point  { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto window_id() const noexcept -> window_id          { return {_event.windowID};                                    }
    auto which()     const noexcept -> u32                { return _event.which;                                         }
    auto state()     const noexcept -> mouse_button_state { return static_cast<mouse_button_state>(_event.state);        }
    auto x()         const noexcept -> i32                { return _event.x;                                             }
    auto y()         const noexcept -> i32                { return _event.y;                                             }
    auto xrel()      const noexcept -> i32                { return _event.xrel;                                          }
    auto yrel()      const noexcept -> i32                { return _event.yrel;                                          }
};

class mouse_button_event {
    SDL_MouseButtonEvent _event;

public:
    auto type()      const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto window_id() const noexcept -> window_id         { return {_event.windowID};                                    }
    auto which()     const noexcept -> u32               { return _event.which;                                         }
    auto button()    const noexcept -> mouse_button      { return mouse_button{_event.button};                          }
    auto x()         const noexcept -> i32               { return _event.x;                                             }
    auto y()         const noexcept -> i32               { return _event.y;                                             }
    auto clicks()    const noexcept -> u8                { return _event.clicks;                                        }
};

class mouse_wheel_event {
    SDL_MouseWheelEvent _event;

public:
    auto type()      const noexcept -> event_type            { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point     { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto window_id() const noexcept -> window_id             { return {_event.windowID};                                    }
    auto which()     const noexcept -> u32                   { return _event.which;                                         }
    auto x()         const noexcept -> i32                   { return _event.x;                                             }
    auto y()         const noexcept -> i32                   { return _event.y;                                             }
    auto direction() const noexcept -> mouse_wheel_direction { return static_cast<mouse_wheel_direction>(_event.direction); }
};

class multi_gesture_event {
    SDL_MultiGestureEvent _event;

public:
    auto type()        const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp()   const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto touch_id()    const noexcept -> sdl::touch_id     { return {_event.touchId};                                     }
    auto rotation()    const noexcept -> float             { return _event.dTheta;                                        }
    auto pinch()       const noexcept -> float             { return _event.dDist;                                         }
    auto x()           const noexcept -> float             { return _event.x;                                             }
    auto y()           const noexcept -> float             { return _event.y;                                             }
    auto num_fingers() const noexcept -> u16               { return _event.numFingers;                                    }
};

class quit_event {
    SDL_QuitEvent _event;

public:
    auto type()      const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
};

class syswm_event {
    SDL_SysWMEvent _event;

public:
    auto type()      const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto message()   const noexcept -> SDL_SysWMmsg*     { return _event.msg;                                           }
};

class text_editing_event {
    SDL_TextEditingEvent _event;

public:
    auto type()      const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto window_id() const noexcept -> window_id         { return {_event.windowID};                                    }
    auto text()      const noexcept -> const char*       { return _event.text;                                          }
    auto start()     const noexcept -> i32               { return _event.start;                                         }
    auto length()    const noexcept -> i32               { return _event.length;                                        }
};

class text_input_event {
    SDL_TextInputEvent _event;

public:
    auto type()      const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto window_id() const noexcept -> window_id         { return {_event.windowID};                                    }
    auto text()      const noexcept -> const char*       { return _event.text;                                          }
};

enum class window_event_type : u8 {
    none         = SDL_WINDOWEVENT_NONE,
    shown        = SDL_WINDOWEVENT_SHOWN,
    hidden       = SDL_WINDOWEVENT_HIDDEN,
    exposed      = SDL_WINDOWEVENT_EXPOSED,
    moved        = SDL_WINDOWEVENT_MOVED,
    resized      = SDL_WINDOWEVENT_RESIZED,
    size_changed = SDL_WINDOWEVENT_SIZE_CHANGED,
    minimized    = SDL_WINDOWEVENT_MINIMIZED,
    maximized    = SDL_WINDOWEVENT_MAXIMIZED,
    restored     = SDL_WINDOWEVENT_RESTORED,
    enter        = SDL_WINDOWEVENT_ENTER,
    leave        = SDL_WINDOWEVENT_LEAVE,
    focus_gained = SDL_WINDOWEVENT_FOCUS_GAINED,
    focus_lost   = SDL_WINDOWEVENT_FOCUS_LOST,
    close        = SDL_WINDOWEVENT_CLOSE,
    take_focus   = SDL_WINDOWEVENT_TAKE_FOCUS,
    hit_test     = SDL_WINDOWEVENT_HIT_TEST
};

class window_event {
    SDL_WindowEvent _event;

public:
    auto type()      const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto event()     const noexcept -> window_event_type { return static_cast<window_event_type>(_event.event);         }
    auto window_id() const noexcept -> window_id         { return {_event.windowID};                                    }
    auto data1()     const noexcept -> i32               { return _event.data1;                                         }
    auto data2()     const noexcept -> i32               { return _event.data2;                                         }
};

class event {
    SDL_Event _event;

public:
    event() noexcept = default;

    auto type() const noexcept -> event_type { return static_cast<event_type>(_event.type); }

    auto audio_device()      const noexcept { return reinterpret_cast<const audio_device_event&      > (_event.adevice);  }
    auto controller_axis()   const noexcept { return reinterpret_cast<const controller_axis_event&   > (_event.caxis);    }
    auto controller_button() const noexcept { return reinterpret_cast<const controller_button_event& > (_event.cbutton);  }
    auto controller_device() const noexcept { return reinterpret_cast<const controller_device_event& > (_event.cdevice);  }
    auto dollar_gesture()    const noexcept { return reinterpret_cast<const dollar_gesture_event&    > (_event.dgesture); }
    auto drop()              const noexcept { return reinterpret_cast<const drop_event&              > (_event.drop);     }
    auto touch_finger()      const noexcept { return reinterpret_cast<const touch_finger_event&      > (_event.tfinger);  }
    auto keyboard()          const noexcept { return reinterpret_cast<const keyboard_event&          > (_event.key);      }
    auto joy_axis()          const noexcept { return reinterpret_cast<const joy_axis_event&          > (_event.jaxis);    }
    auto joy_ball()          const noexcept { return reinterpret_cast<const joy_ball_event&          > (_event.jball);    }
    auto joy_hat()           const noexcept { return reinterpret_cast<const joy_hat_event&           > (_event.jhat);     }
    auto joy_button()        const noexcept { return reinterpret_cast<const joy_button_event&        > (_event.jbutton);  }
    auto joy_device()        const noexcept { return reinterpret_cast<const joy_device_event&        > (_event.jdevice);  }
    auto mouse_motion()      const noexcept { return reinterpret_cast<const mouse_motion_event&      > (_event.motion);   }
    auto mouse_button()      const noexcept { return reinterpret_cast<const mouse_button_event&      > (_event.button);   }
    auto mouse_wheel()       const noexcept { return reinterpret_cast<const mouse_wheel_event&       > (_event.wheel);    }
    auto multi_gesture()     const noexcept { return reinterpret_cast<const multi_gesture_event&     > (_event.mgesture); }
    auto quit()              const noexcept { return reinterpret_cast<const quit_event&              > (_event.quit);     }
    auto syswm()             const noexcept { return reinterpret_cast<const syswm_event&             > (_event.syswm);    }
    auto text_editing()      const noexcept { return reinterpret_cast<const text_editing_event&      > (_event.edit);     }
    auto text_input()        const noexcept { return reinterpret_cast<const text_input_event&        > (_event.text);     }
    auto window()            const noexcept { return reinterpret_cast<const window_event&            > (_event.window);   }
};

// clang-format on

inline void pump_events() noexcept
{
    SDL_PumpEvents();
}

namespace event_type_filter {

inline void set(event_type etype, bool enable) noexcept
{
    const auto type = static_cast<u32>(etype);
    const auto state = static_cast<SDL_bool>(!enable);
    SDL_EventState(type, state);
}

inline auto is_enabled(event_type etype) noexcept -> bool
{
    const auto type = static_cast<u32>(etype);
    return SDL_GetEventState(type) == SDL_ENABLE ? false : true;
}

} // namespace event_type_filter

class event_filter {
public:
    using function_type = bool(const event&);

    event_filter(function_type* f) noexcept
        : _userdata{reinterpret_cast<void*>(f)}
    {
        _filter = [](void* userdata, SDL_Event* e) -> int {
            const auto func = reinterpret_cast<function_type*>(userdata);
            const auto& ev = *reinterpret_cast<event*>(e);
            return static_cast<int>(func(ev));
        };
    }

    template<typename EventFilter>
    event_filter(EventFilter& f) noexcept
        : _userdata{&f}
    {
        static_assert(std::is_invocable_r_v<bool, EventFilter, const event&>);
        _filter = [](void* userdata, SDL_Event* e) -> int {
            auto& func = *static_cast<EventFilter*>(userdata);
            const auto& ev = *reinterpret_cast<event*>(e);
            return static_cast<int>(func(ev));
        };
    }

    event_filter(SDL_EventFilter f, void* userdata) noexcept
        : _filter{f}
        , _userdata{userdata}
    {}

    auto filter() const noexcept -> SDL_EventFilter
    {
        return _filter;
    }

    auto userdata() const noexcept -> void*
    {
        return _userdata;
    }

    auto operator()(const event& e) const noexcept -> bool
    {
        const auto ev = const_cast<SDL_Event*>(reinterpret_cast<const SDL_Event*>(&e));
        return static_cast<bool>(_filter(_userdata, ev));
    }

    friend auto operator==(const event_filter& lhs, const event_filter& rhs) noexcept -> bool
    {
        return lhs.filter() == rhs.filter() && lhs.userdata() == rhs.userdata();
    }

    friend auto operator!=(const event_filter& lhs, const event_filter& rhs) noexcept -> bool
    {
        return !(lhs == rhs);
    }

    static void set(event_filter f) noexcept
    {
        SDL_SetEventFilter(f._filter, f._userdata);
    }

    static auto get() noexcept -> std::optional<event_filter>
    {
        auto filter = SDL_EventFilter{};
        auto userdata = static_cast<void*>(nullptr);
        if (SDL_GetEventFilter(&filter, &userdata)) {
            return event_filter{filter, userdata};
        } else {
            return std::nullopt;
        }
    }

private:
    SDL_EventFilter _filter = nullptr;
    void* _userdata = nullptr;
};

namespace event_watch {

namespace detail {

template<typename EventWatch>
struct watch_generator {
    static auto sdl_callback(void* userdata, SDL_Event* e) -> int
    {
        auto& watch = *static_cast<EventWatch*>(userdata);
        const auto& ev = *reinterpret_cast<event*>(e);
        watch(ev);
        return 0; // return value is ignored by SDL2
    }
};

inline auto fp_sdl_callback(void* userdata, SDL_Event* e) -> int
{
    auto watch = reinterpret_cast<void (*)(const event&)>(userdata);
    const auto& ev = *reinterpret_cast<event*>(e);
    watch(ev);
    return 0; // return value is ignored by SDL2
}

} // namespace detail

template<typename EventWatch>
void add(EventWatch& w)
{
    static_assert(std::is_invocable_r_v<void, EventWatch, const event&>);
    SDL_AddEventWatch(detail::watch_generator<EventWatch>::sdl_callback, &w);
}

inline void add(void (*watch)(const event&))
{
    SDL_AddEventWatch(detail::fp_sdl_callback, reinterpret_cast<void*>(watch));
}

template<typename EventWatch>
void remove(EventWatch& w)
{
    static_assert(std::is_invocable_r_v<void, EventWatch, const event&>);
    SDL_DelEventWatch(detail::watch_generator<EventWatch>::sdl_callback, &w);
}

inline void remove(void (*watch)(const event&))
{
    SDL_DelEventWatch(detail::fp_sdl_callback, reinterpret_cast<void*>(watch));
}

} // namespace event_watch

namespace event_queue {

inline void flush(event_type etype) noexcept
{
    SDL_FlushEvent(static_cast<u32>(etype));
}

inline void flush(event_type min, event_type max)
{
    SDL_FlushEvents(static_cast<u32>(min), static_cast<u32>(max));
}

template<typename EventFilter>
void filter(EventFilter& f)
{
    static_assert(std::is_invocable_r_v<bool, EventFilter, const event&>);
    constexpr auto sdl_callback = [](void* userdata, SDL_Event* e) -> int {
        auto& filter = *static_cast<EventFilter*>(userdata);
        const auto& ev = *reinterpret_cast<event*>(e);
        return static_cast<bool>(filter(ev));
    };
    SDL_FilterEvents(sdl_callback, &f);
}

inline void filter(bool (*filter)(const event&))
{
    constexpr auto fp_sdl_callback = [](void* userdata, SDL_Event* e) -> int {
        auto f = reinterpret_cast<bool (*)(const event&)>(userdata);
        const auto& ev = *reinterpret_cast<event*>(e);
        return static_cast<bool>(f(ev));
    };
    SDL_FilterEvents(fp_sdl_callback, reinterpret_cast<void*>(filter));
}

inline auto has(event_type etype) noexcept -> bool
{
    return SDL_HasEvent(static_cast<u32>(etype));
}

inline auto has(event_type min, event_type max) noexcept -> bool
{
    return SDL_HasEvents(static_cast<u32>(min), static_cast<u32>(max));
}

inline auto was_quit_requested() noexcept -> bool
{
    return SDL_QuitRequested();
}

inline auto push(const event& e) -> bool
{
    const auto result = SDL_PushEvent(reinterpret_cast<SDL_Event*>(const_cast<event*>(&e)));
    if (result < 0) {
        throw error{};
    } else {
        return static_cast<bool>(result);
    }
}

inline auto poll(event& e) noexcept -> bool
{
    return SDL_PollEvent(reinterpret_cast<SDL_Event*>(&e));
}

inline auto add(span<const event> events, event_type min, event_type max) -> int
{
    const auto cevents = const_cast<event*>(events.data());
    const auto p_events = reinterpret_cast<SDL_Event*>(cevents);
    const auto size = static_cast<int>(events.size());
    const auto amin = static_cast<u32>(min);
    const auto amax = static_cast<u32>(max);
    const auto res = SDL_PeepEvents(p_events, size, SDL_ADDEVENT, amin, amax);
    if (res < 0) {
        throw error{};
    } else {
        return res;
    }
}

inline auto peek(span<event> events, event_type min, event_type max) -> int
{
    const auto cevents = const_cast<event*>(events.data());
    const auto p_events = reinterpret_cast<SDL_Event*>(cevents);
    const auto size = static_cast<int>(events.size());
    const auto amin = static_cast<u32>(min);
    const auto amax = static_cast<u32>(max);
    const auto res = SDL_PeepEvents(p_events, size, SDL_PEEKEVENT, amin, amax);
    if (res < 0) {
        throw error{};
    } else {
        return res;
    }
}

inline auto get(span<event> events, event_type min, event_type max) -> int
{
    const auto cevents = const_cast<event*>(events.data());
    const auto p_events = reinterpret_cast<SDL_Event*>(cevents);
    const auto size = static_cast<int>(events.size());
    const auto amin = static_cast<u32>(min);
    const auto amax = static_cast<u32>(max);
    const auto res = SDL_PeepEvents(p_events, size, SDL_GETEVENT, amin, amax);
    if (res < 0) {
        throw error{};
    } else {
        return res;
    }
}

inline auto await(event& e) noexcept -> bool
{
    return SDL_WaitEvent(reinterpret_cast<SDL_Event*>(&e)) == 1;
}

inline auto await(event& e, clock::duration timeout) noexcept -> bool
{
    SDL_WaitEventTimeout(reinterpret_cast<SDL_Event*>(&e), timeout.count());
}

} // namespace event_queue

} // namespace sdl
