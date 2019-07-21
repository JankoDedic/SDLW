#pragma once

#include <SDL2/SDL.h>

#include <sdlw/audio.hpp>
#include <sdlw/time.hpp>
#include <sdlw/video/window.hpp>
#include <sdlw/events/game_controller/game_controller_axis.hpp>
#include <sdlw/events/game_controller/game_controller_button.hpp>
#include <sdlw/events/gesture/gesture_id.hpp>
#include <sdlw/events/joystick/joystick_hat_position.hpp>
#include <sdlw/events/joystick/joystick_instance_id.hpp>
#include <sdlw/events/keyboard/key.hpp>
#include <sdlw/events/mouse/mouse_button_state.hpp>
#include <sdlw/events/mouse/mouse_wheel_direction.hpp>
#include <sdlw/events/touch/touch_device_id.hpp>
#include <sdlw/events/touch/touch_finger.hpp>

namespace sdlw {

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
    auto type()      const noexcept -> event_type              { return static_cast<event_type>(_event.type);                             }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
};

class audio_device_event {
    SDL_AudioDeviceEvent _event;

public:
    auto type()       const noexcept -> event_type              { return static_cast<event_type>(_event.type);                             }
    auto timestamp()  const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto is_capture() const noexcept -> bool                    { return static_cast<bool>(_event.iscapture);                              }
    auto which()      const noexcept -> u32                     { return _event.which;                                                     }
};

class controller_axis_event {
    SDL_ControllerAxisEvent _event;

public:
    auto type()      const noexcept -> event_type           { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point    { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto which()     const noexcept -> joystick_id          { return {_event.which};                                       }
    auto axis()      const noexcept -> game_controller_axis { return static_cast<game_controller_axis>(_event.axis);       }
    auto value()     const noexcept -> i16                  { return _event.value;                                         }
};

class controller_button_event {
    SDL_ControllerButtonEvent _event;

public:
    auto type()      const noexcept -> event_type             { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point      { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto which()     const noexcept -> joystick_id            { return {_event.which};                                       }
    auto button()    const noexcept -> game_controller_button { return static_cast<game_controller_button>(_event.button);   }
};

class controller_device_event {
    SDL_ControllerDeviceEvent _event;

public:
    auto type()      const noexcept -> event_type              { return static_cast<event_type>(_event.type);                             }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto which()     const noexcept -> i32                     { return _event.which;                                                     }
};

class dollar_gesture_event {
    SDL_DollarGestureEvent _event;

public:
    auto type()        const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp()   const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto touch_id()    const noexcept -> sdlw::touch_id    { return {_event.touchId};                                     }
    auto gesture_id()  const noexcept -> gesture_id        { return {_event.gestureId};                                   }
    auto num_fingers() const noexcept -> u32               { return _event.numFingers;                                    }
    auto error()       const noexcept -> float             { return _event.error;                                         }
    auto x()           const noexcept -> float             { return _event.x;                                             }
    auto y()           const noexcept -> float             { return _event.y;                                             }
};

class drop_event {
    SDL_DropEvent _event;

public:
    auto type()      const noexcept -> event_type              { return static_cast<event_type>(_event.type);                             }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto window_id() const noexcept -> window_id        { return {_event.windowID};                                                }
    auto file()      const noexcept -> const char*             { return _event.file;                                                      }
};

class touch_finger_event {
    SDL_TouchFingerEvent _event;

public:
    auto type()      const noexcept -> event_type        { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto touch_id()  const noexcept -> sdlw::touch_id    { return {_event.touchId};                                     }
    auto finger_id() const noexcept -> sdlw::finger_id   { return {_event.fingerId};                                    }
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
    auto window_id() const noexcept -> window_id  { return {_event.windowID};                                    }
    auto key()       const noexcept -> sdlw::key         { return sdlw::key{_event.keysym};                             }
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
    auto type()      const noexcept -> event_type              { return static_cast<event_type>(_event.type);                             }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto which()     const noexcept -> i32                     { return _event.which;                                                     }
};

class mouse_motion_event {
    SDL_MouseMotionEvent _event;

public:
    auto type()      const noexcept -> event_type         { return static_cast<event_type>(_event.type);                 }
    auto timestamp() const noexcept -> clock::time_point  { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto window_id() const noexcept -> window_id   { return {_event.windowID};                                    }
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
    auto window_id() const noexcept -> window_id  { return {_event.windowID};                                    }
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
    auto window_id() const noexcept -> window_id      { return {_event.windowID};                                    }
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
    auto touch_id()    const noexcept -> sdlw::touch_id    { return {_event.touchId};                                     }
    auto rotation()    const noexcept -> float             { return _event.dTheta;                                        }
    auto pinch()       const noexcept -> float             { return _event.dDist;                                         }
    auto x()           const noexcept -> float             { return _event.x;                                             }
    auto y()           const noexcept -> float             { return _event.y;                                             }
    auto num_fingers() const noexcept -> u16               { return _event.numFingers;                                    }
};

class quit_event {
    SDL_QuitEvent _event;

public:
    auto type()      const noexcept -> event_type              { return static_cast<event_type>(_event.type);                             }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
};

class syswm_event {
    SDL_SysWMEvent _event;

public:
    auto type()      const noexcept -> event_type              { return static_cast<event_type>(_event.type);                             }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto message()   const noexcept -> SDL_SysWMmsg*           { return _event.msg;                                                       }
};

class text_editing_event {
    SDL_TextEditingEvent _event;

public:
    auto type()      const noexcept -> event_type              { return static_cast<event_type>(_event.type);                             }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto window_id() const noexcept -> window_id        { return {_event.windowID};                                                }
    auto text()      const noexcept -> const char*             { return _event.text;                                                      }
    auto start()     const noexcept -> i32                     { return _event.start;                                                     }
    auto length()    const noexcept -> i32                     { return _event.length;                                                    }
};

class text_input_event {
    SDL_TextInputEvent _event;

public:
    auto type()      const noexcept -> event_type              { return static_cast<event_type>(_event.type);                             }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto window_id() const noexcept -> window_id        { return {_event.windowID};                                                }
    auto text()      const noexcept -> const char*             { return _event.text;                                                      }
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
    auto type()      const noexcept -> event_type              { return static_cast<event_type>(_event.type);                             }
    auto timestamp() const noexcept -> clock::time_point { return clock::time_point{clock::duration{_event.timestamp}}; }
    auto event()     const noexcept -> window_event_type       { return static_cast<window_event_type>(_event.event);                     }
    auto window_id() const noexcept -> window_id        { return {_event.windowID};                                                }
    auto data1()     const noexcept -> i32                     { return _event.data1;                                                     }
    auto data2()     const noexcept -> i32                     { return _event.data2;                                                     }
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

} // namespace sdlw
