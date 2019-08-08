#pragma once

#include <SDL2/SDL_rwops.h>

#include <sdlw/error.hpp>
#include <sdlw/types.hpp>

namespace sdlw {

class stream {
public:
    stream() noexcept
    {
        _rwops.size = [](SDL_RWops* context) -> i64 {
            const auto& s = *reinterpret_cast<stream*>(context);
            return s.size();
        };

        _rwops.seek = [](SDL_RWops* context, i64 offset, int whence) -> i64 {
            auto& s = *reinterpret_cast<stream*>(context);
            return s.seek(offset, whence);
        };

        _rwops.read = [](SDL_RWops* context, void* ptr, std::size_t size, std::size_t maxnum) -> std::size_t {
            auto& s = *reinterpret_cast<stream*>(context);
            return s.read(ptr, size, maxnum);
        };

        _rwops.write = [](SDL_RWops* context, const void* ptr, std::size_t size, std::size_t num) -> std::size_t {
            auto& s = *reinterpret_cast<stream*>(context);
            return s.write(ptr, size, num);
        };

        _rwops.close = [](SDL_RWops* context) -> int {
            auto& s = *reinterpret_cast<stream*>(context);
            return s.close();
        };
    }

    stream(const stream&) = delete;
    auto operator=(const stream&) -> stream& = delete;
    stream(stream&&) = delete;
    auto operator=(stream &&) -> stream& = delete;

    auto get_pointer() noexcept -> SDL_RWops*
    {
        return &_rwops;
    }

    virtual auto size() const -> i64 = 0;
    virtual auto seek(i64 offset, int whence) -> i64 = 0;
    virtual auto read(void* ptr, std::size_t size, std::size_t maxnum) -> std::size_t = 0;
    virtual auto write(const void* ptr, std::size_t size, std::size_t num) -> std::size_t = 0;
    virtual auto close() -> int = 0;

    auto tell() -> i64
    {
        return SDL_RWtell(&_rwops);
    }

private:
    SDL_RWops _rwops = {};
};

class file_stream : public stream {
public:
    file_stream(const char* file, const char* mode)
        : _file_stream{SDL_RWFromFile(file, mode)}
    {
        if (!_file_stream) throw error{};
    }

    file_stream(FILE* fp, bool autoclose)
        : _file_stream{SDL_RWFromFP(fp, static_cast<SDL_bool>(autoclose))}
    {
        if (!_file_stream) throw error{};
    }

    auto size() const -> i64 override
    {
        return SDL_RWsize(_file_stream);
    }

    auto seek(i64 offset, int whence) -> i64 override
    {
        return SDL_RWseek(_file_stream, offset, whence);
    }

    auto read(void* ptr, std::size_t size, std::size_t maxnum) -> std::size_t override
    {
        return SDL_RWread(_file_stream, ptr, size, maxnum);
    }

    auto write(const void* ptr, std::size_t size, std::size_t num) -> std::size_t override
    {
        return SDL_RWwrite(_file_stream, ptr, size, num);
    }

    auto close() -> int override
    {
        return SDL_RWclose(_file_stream);
    }

private:
    SDL_RWops* _file_stream = nullptr;
};

class memory_stream : public stream {
public:
    memory_stream(const void* mem, int size)
        : _memory_stream(SDL_RWFromConstMem(mem, size))
    {
        if (!_memory_stream) throw error{};
    }

    memory_stream(void* mem, int size)
        : _memory_stream(SDL_RWFromMem(mem, size))
    {
        if (!_memory_stream) throw error{};
    }

    auto size() const -> i64 override
    {
        return SDL_RWsize(_memory_stream);
    }

    auto seek(i64 offset, int whence) -> i64 override
    {
        return SDL_RWseek(_memory_stream, offset, whence);
    }

    auto read(void* ptr, std::size_t size, std::size_t maxnum) -> std::size_t override
    {
        return SDL_RWread(_memory_stream, ptr, size, maxnum);
    }

    auto write(const void* ptr, std::size_t size, std::size_t num) -> std::size_t override
    {
        return SDL_RWwrite(_memory_stream, ptr, size, num);
    }

    auto close() -> int override
    {
        return SDL_RWclose(_memory_stream);
    }

private:
    SDL_RWops* _memory_stream = nullptr;
};

} // namespace sdlw
