// SPDX-License-Identifier: MIT

#ifndef MTEA_EXCEPT_H
#define MTEA_EXCEPT_H

#ifdef MTEA_USE_FULL_LIB

#include <stdexcept>
#include <string>

namespace mtea {

class block_error : public std::runtime_error {
public:
    explicit block_error(const char* msg) : std::runtime_error(msg) {}
    explicit block_error(const std::string& msg) : std::runtime_error(msg) {}
};

}

#endif // MTEA_USE_FULL_LIB

#endif // MTEA_EXCEPT_H
