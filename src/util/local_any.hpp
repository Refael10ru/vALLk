#pragma once

#include <array>
#include <cassert>

namespace vallk::util {

//TODO: noexcepts, assignment and emplace and ctor from T, trivial type special treatment(need an extra member flag), decay perhaps
template<std::size_t Size>
class local_any {
public:
    local_any() = default;
    ~local_any() { reset(); }
    local_any(const local_any& oth) { *this = oth; }
    local_any(local_any&& oth) { *this = std::move(oth); }

    local_any& operator=(const local_any& oth) {
        reset();

        _local_rtti = oth._local_rtti;
        _local_rtti._copy(_memory.data(), oth._memory.data());
        return *this;
    }
    local_any& operator=(local_any&& oth) {
        reset();
        
        _local_rtti = oth._local_rtti;
        _local_rtti._move(_memory.data(), oth._memory.data());

        oth._local_rtti._destructor = nullptr;
        oth._local_rtti._move = nullptr;
        oth._local_rtti._copy = nullptr;
        return *this;
    }

    // NOTE: basic exception guarantee due to in-place construction
    template<class T, class... Args>
    void emplace(Args&&... args) {
        reset();

        std::construct_at<T>(reinterpret_cast<T*>(_memory.data()), std::forward<Args>(args)...);
        _local_rtti._destructor = [](void* ptr) { static_cast<T*>(ptr)->~T(); };
        _local_rtti._move = [](void* dst, void* src) { std::construct_at<T>(reinterpret_cast<T*>(dst), std::move(*static_cast<T*>(src))); };
        _local_rtti._copy = [](void* dst, const void* src) { std::construct_at<T>(reinterpret_cast<T*>(dst), *static_cast<const T*>(src)); };
    }

    template<class T>
    T& cast() {
        assert(_local_rtti._destructor != nullptr);
        return *reinterpret_cast<T*>(_memory.data());
    }
    template<class T>
    const T& cast() const {
        assert(_local_rtti._destructor != nullptr);
        return *reinterpret_cast<const T*>(_memory.data());
    }

    void reset() {
        if (_local_rtti._destructor != nullptr) {
            _local_rtti._destructor(_memory.data());
        }
        _local_rtti._destructor = nullptr;
        _local_rtti._move = nullptr;
        _local_rtti._copy = nullptr;
    }

private:
    using destructor_t = void (*)(void*);
    using move_t = void (*)(void*, void*);
    using copy_t = void (*)(void*, const void*);

    struct {
        destructor_t _destructor = nullptr;
        move_t _move = nullptr;
        copy_t _copy = nullptr;
    } _local_rtti;

    std::array<std::byte, Size> _memory;
};

}