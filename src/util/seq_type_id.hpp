#pragma once

#include <cstdint>
// shameless copypaste from dry
namespace vallk::util {

using type_id_t = std::uint32_t;

template<typename Owner>
struct type_id_iterator {
    static type_id_t advance() {
        static type_id_t value{};
        return value++;
    }
};

template<typename T, typename Owner>
struct seq_type_id {
    static type_id_t value() {
        static const type_id_t ret = type_id_iterator<Owner>::advance();
        return ret;
    }
};

}
