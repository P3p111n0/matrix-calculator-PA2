#pragma once

#include "../../matrix_wrapper/Matrix.h"
#include "../../matrix_wrapper/MatrixFactory.h"
#include "../BaseHandler.h"
#include <unordered_map>

/**
 * @brief A base class for classes handling user input. Also used for
 *        synchronizing patterns of temporary variables. Provides a matrix
 *        factory for its derived classes.
 */
class InputHandler : public BaseHandler {
  public:

    /**
     * @brief Initializes the handler.
     * @param factory Factory used for creating temporary matrices in derived
     *                classes.
     */
    explicit InputHandler(MatrixFactory factory);
  protected:

    /**
     * @brief Checks if <b>string</b> has a prefix equal to <b>prefix</b>.
     * @param string String to be examined.
     * @param prefix Prefix to look for.
     * @return True if <b>string</b> has a prefix equal to <b>prpefix</b>,
     *         false otherwise.
     */
    static bool string_has_prefix(const std::string & string,
                                  const std::string & prefix);

    /**
     * @brief A method for creating temporary names of variables. Essential
     *        for correct cooperation of parsing and evaluating units.
     *        A temporary name consists of 3 parts, in this order:\n
     *        Prefix: See static variable <b>RESERVED_NAME_PREFIX</b>.
     *        Base: The provided string <b>base</b>.
     *        Count: a number unique for every name created <b>by the same
     *        object</b>. Uniqueness cannot be guaranteed across all instances.
     * @param base A base string used for the temporary name.
     * @return A name for a temporary variable.
     */
    std::string get_temporary_name(const std::string & base) const;

    /**
     * @brief A variable used for keeping prefixes consistent across all
     *        instances of this class.
     */
    static constexpr char RESERVED_NAME_PREFIX[] = "__";
};
