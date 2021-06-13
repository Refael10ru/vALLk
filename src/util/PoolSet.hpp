#pragma once

#include <vector>

#include "LocalAny.hpp"
#include "SeqTypeID.hpp"

namespace vallk::util {

template<class Pair_Type = void>
class PoolSet {
public:
    template<class T>
    using PoolUnderlying = std::vector<T>;
    template<class T>
    using PoolType = std::conditional_t<std::is_void_v<Pair_Type>, PoolUnderlying<T>, std::pair<PoolUnderlying<T>, Pair_Type>>;

    template<class T>
    PoolType<T>& getPool() {
        return securePool<T>();
    }
    // NOTE: const won't create a new one - out of bounds access if pool not allocated
    template<class T>
    const PoolType<T>& getPool() const {
        const auto tID = PoolTypeID<T>::value();
        return _pools[tID].cast<PoolType<T>>();
    }

private:
    template<class T>
    using PoolTypeID = util::SeqTypeID<T, PoolSet>;

    static constexpr std::size_t _anySizeEstimate = sizeof(PoolType<int>);

    template<class T>
    PoolType<T>& securePool() {
        const auto tID = PoolTypeID<T>::value();

        if (tID >= _pools.size()) {
            _pools.resize(tID + 1);
            _pools[tID].emplace<PoolType<T>>(PoolType<T>{});
        }
        return _pools[tID].cast<PoolType<T>>();
    }

    std::vector<LocalAny<_anySizeEstimate>> _pools;
};

}