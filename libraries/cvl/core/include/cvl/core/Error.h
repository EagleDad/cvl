#pragma once

// STD includes
#include <iostream>

// CVL includes
#include <cvl/core/export.h>

namespace cvl::core
{
/**
 * @brief This class describes the Error class
 *
 *
 *
 */
//#pragma warning( push )
//#pragma warning( disable : 4275 )
//#pragma warning( disable : 4251 )
class CVL_CORE_EXPORT Error final : public std::exception
{
public:
    /**
     * Value construct
     *
     * @param file   The file where the error occurred
     * @param line   The line where the error occurred
     */
    Error( const char* file, uint32_t line );

    /**
     * Value construct
     *
     * @param file      The file where the error occurred
     * @param line      The line where the error occurred
     * @param message   A more precise error message
     */
    Error( const char* file, uint32_t line, std::string message );

    /**
     * Get line number
     */
    [[nodiscard]] auto getLine( ) const -> uint32_t;

    /**
     * Get file name
     */
    [[nodiscard]] auto getFileName( ) const -> std::string;

    /**
     * Get the error message
     */
    [[nodiscard]] auto getMessage( ) const -> std::string;

    /**
     * Output operator
     *
     * @param os     The stream to write to
     * @param error  The error object to write
     */
    CVL_CORE_EXPORT
    friend auto operator<<( std::ostream& os, const Error& error )
        -> std::ostream&;

    /**
     * Print exception content
     */
    [[nodiscard]] char const* what( ) const noexcept override;

private:
    std::string mFile;
    unsigned int mLine;
    std::string mMessage;
};
//#pragma warning( pop )
} // namespace cvl::core
