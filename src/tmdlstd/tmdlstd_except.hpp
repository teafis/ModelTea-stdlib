// SPDX-License-Identifier: MIT

#ifndef TMDL_STDLIB_EXCEPT_H
#define TMDL_STDLIB_EXCEPT_H

#include <stdexcept>
#include <string>

namespace tmdl {
namespace stdlib {

class block_error : public std::runtime_error {
public:
    block_error(const char* msg) : std::runtime_error(msg) {}
    block_error(const std::string& msg) : std::runtime_error(msg) {}
};

}
}

#endif // TMDL_STDLIB_EXCEPT_H
