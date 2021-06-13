#pragma once

#include <cstdint>
// shameless copypaste from dry
namespace vallk::util {

using TypeIdType = std::uint32_t;

template<typename Owner>
struct TypeIDIterator {
    static TypeIdType advance() {
        static TypeIdType value{};
        return value++;
    }
};

template<typename T, typename Owner>
struct SeqTypeID {
    static TypeIdType value() {
        static const TypeIdType ret = TypeIDIterator<Owner>::advance();
        return ret;
    }
};

}
