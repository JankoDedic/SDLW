#pragma once

#include <sdlw/video/renderer.hpp>
#include <sdlw/video/texture.hpp>

namespace sdlw::video {

inline
const texture&
renderer::target() const
{
    static auto s = ::sdlw::detail::storage<texture>();
    if (const auto ptr = SDL_GetRenderTarget(get_pointer())) {
        return *new (&s) texture(ptr);
    } else {
        throw error();
    }
}

inline
texture&
renderer::target()
{
    static auto s = ::sdlw::detail::storage<texture>();
    if (const auto ptr = SDL_GetRenderTarget(get_pointer())) {
        return *new (&s) texture(ptr);
    } else {
        throw error();
    }
}

inline
void
renderer::set_target(texture* target)
{
    const auto ptarget = target ? target->get_pointer() : nullptr;
    if (SDL_SetRenderTarget(get_pointer(), ptarget) < 0) {
        throw error();
    }
}

inline
void
renderer::copy(
    const texture& tex,
    const rectangle* source,
    const rectangle* destination)
{
    const auto ptexture = tex.get_pointer();
    if (SDL_RenderCopy(get_pointer(), ptexture, source, destination) < 0) {
        throw error();
    }
}

inline
void
renderer::copy(
    const texture& tex,
    const rectangle* source,
    const rectangle* destination,
    double angle,
    const point* center,
    renderer_flip flip)
{
    const auto ptr = get_pointer();
    const auto ptex = tex.get_pointer();
    const auto src = source;
    const auto dst = destination;
    const auto flip_ = static_cast<SDL_RendererFlip>(flip);
    if (SDL_RenderCopyEx(ptr, ptex, src, dst, angle, center, flip_) < 0) {
        throw error();
    }
}

inline
texture::texture(
    const renderer& rend,
    pixels::pixel_format_type format,
    texture_access access,
    const sdlw::video::size& sz)
{
    const auto prend = rend.get_pointer();
    const auto format_ = static_cast<u32>(format);
    const auto access_ = static_cast<int>(access);
    const auto ptr = SDL_CreateTexture(prend, format_, access_, sz.w, sz.h);
    if (ptr) {
        *this = texture(ptr);
    } else {
        throw error();
    }
}

inline
texture::texture(const renderer& rend, const surface& surf)
{
    const auto prend = rend.get_pointer();
    const auto psurf = surf.get_pointer();
    const auto ptr = SDL_CreateTextureFromSurface(prend, psurf);
    if (ptr) {
        *this = texture(ptr);
    } else {
        throw error();
    }
}

} // namespace sdlw::video
