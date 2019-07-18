#pragma once

#include <SDL2/SDL_assert.h>

#ifndef SDL_ASSERT_LEVEL
#   define SDL_ASSERT_LEVEL SDLW_ASSERT_LEVEL
#endif

#define SDLW_ASSERT          SDL_assert
#define SDLW_ASSERT_RELEASE  SDL_assert_release
#define SDLW_ASSERT_PARANOID SDL_assert_paranoid
