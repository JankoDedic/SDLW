# =============================================================================
# CMake
# =============================================================================
cmake_minimum_required(VERSION 3.11)

# =============================================================================
# Project
# =============================================================================
project(SDLW)

# =============================================================================
# Options
# =============================================================================
option(SDLW_BUILD_EXAMPLE "Build the example" ON)

# =============================================================================
# Dependencies
# =============================================================================
# SDL2
find_package(SDL2 REQUIRED)

# SDL2_image
find_path    ( SDL2_IMAGE_INCLUDE_DIR SDL2/SDL_image.h )
find_library ( SDL2_IMAGE_LIBRARY     SDL2_image       )

# SDL2_ttf
find_path    ( SDL2_TTF_INCLUDE_DIR   SDL2/SDL_ttf.h   )
find_library ( SDL2_TTF_LIBRARY       SDL2_ttf         )

# span
set(SPAN_LITE_OPT_BUILD_TESTS OFF)
add_subdirectory(third_party/span-lite EXCLUDE_FROM_ALL)

# =============================================================================
# Library
# =============================================================================
file(GLOB_RECURSE SDLW_HEADER_FILES include/*.hpp)

add_library(SDLW INTERFACE)
target_compile_features(SDLW INTERFACE cxx_std_17)
# target_compile_options(SDLW INTERFACE /Za)
target_include_directories(SDLW
  INTERFACE
    include
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
)
target_link_libraries(SDLW
  INTERFACE
    SDL2::SDL2
    SDL2::SDL2main
    ${SDL2_IMAGE_LIBRARY}
    ${SDL2_TTF_LIBRARY}
    span-lite
)

# =============================================================================
# Example
# =============================================================================
if(SDLW_BUILD_EXAMPLE)
  add_executable(main example/example.cpp)
  target_link_libraries(main PRIVATE SDLW)
endif()
