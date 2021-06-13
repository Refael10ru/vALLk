#pragma once

#include <vector>

#include "local_any.hpp"
#include "seq_type_id.hpp"

namespace vallk::util {

template<class U>
class pool_set
{
public:
    template<class T>
    using pool_t = std::pair<std::vector<T>,U>;

    template<class T>
    pool_t<T>& get_pool() {
        return secure_pool<T>();
    }
    template<class T>
    const pool_t<T>& get_pool() const {
        return secure_pool<T>();
    }

private:
    template<class T>
    using pool_type_id = util::seq_type_id<T, pool_set>;

    static constexpr std::size_t _any_size_estimate = sizeof(pool_t<int>) + sizeof(U);

    template<class T>
    pool_t<T>& secure_pool() {
        const auto t_id = pool_type_id<T>::value();

        if (t_id >= _pools.size()) {
            _pools.resize(t_id + 1);
            _pools[t_id].emplace<pool_t<T>>(pool_t<T>{});
        }
        return _pools[t_id].cast<pool_t<T>>();
    }

    std::vector<std::pair<local_any<_any_size_estimate>,U>> _pools;
};



class pool_set<void> 
{
public:
    template<class T>
    using pool_t = std::vector<T>;

    template<class T>
    pool_t<T>& get_pool() {
        return secure_pool<T>();
    }
    template<class T>
    const pool_t<T>& get_pool() const {
        return secure_pool<T>();
    }

private:
    template<class T>
    using pool_type_id = util::seq_type_id<T, pool_set>;

    static constexpr std::size_t _any_size_estimate = sizeof(pool_t<int>);

    template<class T>
    pool_t<T>& secure_pool() {
        const auto t_id = pool_type_id<T>::value();

        if (t_id >= _pools.size()) {
            _pools.resize(t_id + 1);
            _pools[t_id].emplace<pool_t<T>>(pool_t<T>{});
        }
        return _pools[t_id].cast<pool_t<T>>();
    }

    std::vector<local_any<_any_size_estimate>> _pools;
};

}