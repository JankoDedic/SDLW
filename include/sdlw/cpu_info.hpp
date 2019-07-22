#pragma once

#include <SDL2/SDL_cpuinfo.h>

namespace sdlw {

struct cpu_info {
    static auto cache_line_size() noexcept -> int  { return SDL_GetCPUCacheLineSize(); }
    static auto cpu_count()       noexcept -> int  { return SDL_GetCPUCount();         }
    static auto system_ram()      noexcept -> int  { return SDL_GetSystemRAM();        }
    static auto has_3dnow()       noexcept -> bool { return SDL_Has3DNow();            }
    static auto has_avx()         noexcept -> bool { return SDL_HasAVX();              }
    static auto has_avx2()        noexcept -> bool { return SDL_HasAVX2();             }
    static auto has_altivec()     noexcept -> bool { return SDL_HasAltiVec();          }
    static auto has_mmx()         noexcept -> bool { return SDL_HasMMX();              }
    static auto has_rdtsc()       noexcept -> bool { return SDL_HasRDTSC();            }
    static auto has_sse()         noexcept -> bool { return SDL_HasSSE();              }
    static auto has_sse2()        noexcept -> bool { return SDL_HasSSE2();             }
    static auto has_sse3()        noexcept -> bool { return SDL_HasSSE3();             }
    static auto has_sse41()       noexcept -> bool { return SDL_HasSSE41();            }
    static auto has_sse42()       noexcept -> bool { return SDL_HasSSE42();            }
};

} // namespace sdlw
