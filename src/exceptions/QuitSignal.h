#pragma once

#include <exception>

/**
 * @brief Represents a special case of user input, where the user requests
 *        a termination of the app.
 */
class QuitSignal : public std::exception {};