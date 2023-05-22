#include "InputHandler.h"
#include <string>

InputHandler::InputHandler(MatrixFactory factory) : _factory(factory) {}

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

std::string InputHandler::get_temporary_name(const std::string & name_body, std::size_t postfix_num) {
    std::string new_name = std::string(RESERVED_NAME_PREFIX) + name_body;
    new_name += std::to_string(postfix_num);
    return new_name;
}

void InputHandler::remove_temporary_vars(InputHandler::VariableMap & vars) {
    std::vector<std::string> vars_to_delete;
    for (const auto & [key, val] : vars){
        if (string_has_prefix(key, RESERVED_NAME_PREFIX)){
            vars_to_delete.emplace_back(key);
        }
    }
    for (const auto & var_name : vars_to_delete){
        vars.erase(var_name);
    }
}
