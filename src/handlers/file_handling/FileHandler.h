#pragma once

#include "../BaseHandler.h"
#include <string>

/**
 * @brief A base class for classes handling file operations. Provides a way
 *        for reporting their status.
 */
class FileHandler : public BaseHandler {
  public:

    /**
     * @brief Initializes the handler.
     * @param factory A factory used for creating matrices or writing them
     *                to files.
     */
    FileHandler(MatrixFactory factory);

    /**
     * @brief Reports whether or not the last I/O operation failed.
     * @return True if the last I/O operation was successful, false otherwise.
     */
    bool good() const;

    /**
     * @brief Reports whether or not the last I/O operation failed.
     * @return True if the last I/O operation failed, false otherwise.
     */
    bool bad() const;

    /**
     * @brief Serves as a way for the handler to report the details of the
     *        last I/O operation.
     * @return A string containing a status report regarding the last I/O
     *         operation.
     */
    const std::string & status() const;
  protected:

    /**
     * @brief Contains details regarding the last I/O operation.
     */
    std::string _status;

    /**
     * @brief A helper bool for status reporting.
     */
    bool _is_failed;

    /**
     * @brief Resets the handler to a base status. Clears the status string and
     *        sets the handler to a 'not failed' state.
     */
    void reset();
};
