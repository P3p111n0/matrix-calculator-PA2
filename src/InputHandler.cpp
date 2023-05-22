#include "InputHandler.h"

InputHandler::InputHandler(MatrixFactory factory) : _factory(factory) {}

bool InputHandler::string_has_prefix(const std::string & src,
                                     const std::string & prefix) {
    if (src.length() < prefix.length()) {
        return false;
    }

    for (auto src_it = src.begin(), prefix_it = prefix.begin();
         src_it != src.end() && prefix_it != prefix.end();
         ++src_it, ++prefix_it) {
        if (*src_it != *prefix_it){
            return false;
        }
    }
    return true;
}
