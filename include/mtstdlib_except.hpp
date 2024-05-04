// SPDX-License-Identifier: MIT

#ifndef MT_STDLIB_EXCEPT_H
#define MT_STDLIB_EXCEPT_H

#ifdef MT_STDLIB_USE_FULL_LIB

#include <stdexcept>
#include <string>

namespace mt {
namespace stdlib {

class block_error : public std::runtime_error {
public:
    explicit block_error(const char* msg) : std::runtime_error(msg) {}
    explicit block_error(const std::string& msg) : std::runtime_error(msg) {}
};

}
}

#endif // MT_STDLIB_USE_FULL_LIB

#endif // MT_STDLIB_EXCEPT_H
