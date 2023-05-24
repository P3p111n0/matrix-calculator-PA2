#include "InputHandler.h"
#include <string>

InputHandler::InputHandler(MatrixFactory factory) : BaseHandler(factory) {}

bool InputHandler::string_has_prefix(const std::string & src,
                                     const std::string & prefix) {
    if (src.length() < prefix.length()) {
        return false;
    }

    for (auto src_it = src.begin(), prefix_it = prefix.begin();
         src_it != src.end() && prefix_it != prefix.end();
         ++src_it, ++prefix_it) {
        if (*src_it != *prefix_it) {
            return false;
        }
    }
    return true;
}

std::string InputHandler::get_temporary_name(const std::string & name_body) const {
    static size_t tmp_cnt = 0;
    std::string new_name = std::string(RESERVED_NAME_PREFIX) + name_body;
    new_name += std::to_string(tmp_cnt);
    ++tmp_cnt;
    return new_name;
}