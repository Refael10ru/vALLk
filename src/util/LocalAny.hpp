#pragma once

#include <array>
#include <cassert>

namespace vallk::util {

// TODO: decay perhaps
// NOTE: trivial types are treated as if non-trivial
//   rationale: using any on large collections of trivial types is a pessimization anyway
template<std::size_t Size, bool Force_Nothrow_Move = false>
class LocalAny {
public:
    template<class T>
    static constexpr bool TypeFits = sizeof(T) <= Size;

    LocalAny() noexcept = default;
    ~LocalAny() { reset(); }
    LocalAny(const LocalAny& oth) { *this = oth; }
    LocalAny(LocalAny&& oth) { *this = std::move(oth); }

    LocalAny& operator=(const LocalAny& oth) {
        reset();

        _localRTTI = oth._localRTTI;
        _localRTTI._copy(_memory.data(), oth._memory.data());
        return *this;
    }
    LocalAny& operator=(LocalAny&& oth) noexcept(Force_Nothrow_Move) {
        reset();
        
        _localRTTI = oth._localRTTI;
        _localRTTI._move(_memory.data(), oth._memory.data());

        oth._localRTTI._destructor = nullptr;
        oth._localRTTI._move = nullptr;
        oth._localRTTI._copy = nullptr;
        return *this;
    }
    template<class T> requires TypeFits<T>
    LocalAny& operator=(T&& val) {
        reset();

        emplace<T>(std::forward<T>(val));
        return *this;
    }

    // NOTE: basic exception guarantee due to in-place construction
    template<class T, class... Args> requires TypeFits<T>
    void emplace(Args&&... args) {
        reset();

        std::construct_at<T>(reinterpret_cast<T*>(_memory.data()), std::forward<Args>(args)...);
        _localRTTI._destructor = [](void* ptr) { static_cast<T*>(ptr)->~T(); };
        _localRTTI._move = [](void* dst, void* src) { std::construct_at<T>(reinterpret_cast<T*>(dst), std::move(*static_cast<T*>(src))); };
        _localRTTI._copy = [](void* dst, const void* src) { std::construct_at<T>(reinterpret_cast<T*>(dst), *static_cast<const T*>(src)); };
    }

    template<class T> requires TypeFits<T>
    T& cast() {
        assert(_localRTTI._destructor != nullptr);
        return *reinterpret_cast<T*>(_memory.data());
    }
    template<class T> requires TypeFits<T>
    const T& cast() const {
        assert(_localRTTI._destructor != nullptr);
        return *reinterpret_cast<const T*>(_memory.data());
    }

    void reset() {
        if (_localRTTI._destructor != nullptr) {
            _localRTTI._destructor(_memory.data());
        }
        _localRTTI._destructor = nullptr;
        _localRTTI._move = nullptr;
        _localRTTI._copy = nullptr;
    }

private:
    using Destructor = void (*)(void*);
    using MoveCtor = void (*)(void*, void*);
    using CopyCtor = void (*)(void*, const void*);

    struct {
        Destructor _destructor = nullptr;
        MoveCtor _move = nullptr;
        CopyCtor _copy = nullptr;
    } _localRTTI;

    std::array<std::byte, Size> _memory;
};

}