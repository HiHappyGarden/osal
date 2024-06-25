/***************************************************************************
 *
 * OSAL
 * Copyright (C) 2023/2024 Antonio Salsi <passy.linux@zresa.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************/
#pragma once

#include "osal/types.hpp"
#include "osal/memory.hpp"

#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define OSAL_ERROR_BUILD(msg, ...) new osal::error(msg, __VA_ARGS__)
#define OSAL_ERROR_APPEND(old_error, msg, ...) new osal::error(old_error, msg, __VA_ARGS__)
#define OSAL_ERROR_SET_POSITION(obj) obj.set_position(osal::get_file_name(__FILE__), __func__, __LINE__)
#define OSAL_ERROR_PTR_SET_POSITION(obj) (obj)->set_position(osal::get_file_name(__FILE__), __func__, __LINE__)

namespace osal
{
inline namespace v1
{


/**
 * @brief Returns the offset of the file name within the path.
 *
 * This function returns the offset of the file name within the provided path.
 * If the path ends with '/' or '\\', the offset will be the character following the separator.
 * If the path doesn't contain any separators, the offset will be 0.
 *
 * @tparam T The type of the path (character array).
 * @tparam S The size of the character array.
 * @param str The path from which to get the file name offset.
 * @param i The current index in the path (default value: S - 1).
 * @return The offset of the file name within the path.
 */
template <typename T, size_t S>
inline constexpr size_t get_file_name_offset(const T (& str)[S], size_t i = S - 1) OSAL_NOEXCEPT
{
    return (str[i] == '/' || str[i] == '\\') ? i + 1 : (i > 0 ? get_file_name_offset(str, i - 1) : 0);
}

/**
 * @brief Returns the size of the file name within the path.
 *
 * This function returns the size of the file name within the provided path.
 * The size of the file name is calculated as the difference between the total length of the path
 * and the offset of the file name.
 *
 * @tparam T The type of the path (character array).
 * @tparam S The size of the character array.
 * @param str The path from which to get the file name size.
 * @param i The current index in the path (default value: S - 1).
 * @return The size of the file name within the path.
 */
template <typename T, size_t S>
inline constexpr size_t get_file_name_size(const T (& str)[S], size_t i = S - 1) OSAL_NOEXCEPT
{
    return strlen(str) - get_file_name_offset(str);
}

/**
     * @brief Returns the file name within the path.
     *
     * This function returns a pointer to the file name within the provided path.
     * The obtained pointer directly points to the starting character of the file name in the path.
     *
     * @tparam T The type of the path (character array).
     * @tparam S The size of the character array.
     * @param str The path from which to get the file name.
     * @return A pointer to the file name within the path.
     */
template <typename T, size_t S>
inline constexpr const char* get_file_name(const T (& str)[S]) OSAL_NOEXCEPT
{
    return str + get_file_name_offset(str);
}

template <size_t Size>
class string;

/**
 * @class error
 *
 * @brief Represents an error with associated information.
 *
 * The error class represents an error with a message, code, file name, function name, and line number.
 * It also provides functionality to add additional errors and set the position information.
 */
class error final
{
public:
    using ptr = unique_ptr<error>;

private:
    char msg[128]{0};               ///< The error message.
    uint8_t code = 0;               ///< The error code.
    char file[64]{0};               ///< The file name where the error occurred.
    char func[64]{0};               ///< The function name where the error occurred.
    uint32_t line = 0;              ///< The line number where the error occurred.
    error::ptr old_error;           ///< Pointer to the previous error.

    friend void printf_stack_error(const char* app_tag, const error &e, const char* fmt, ...) OSAL_NOEXCEPT;

public:

    /**
     * @brief Default constructor.
     */
    error() = default;

    /**
     * @brief Constructs an error object with a null pointer.
     *
     * @param nullptr_t A null pointer to indicate no error.
     */
    explicit error(nullptr_t) OSAL_NOEXCEPT {}

    /**
     * @brief Constructs an error object with the given message, code, file, function, and line number.
     *
     * @param msg The error message.
     * @param code The error code.
     * @param file The file name where the error occurred.
     * @param func The function name where the error occurred.
     * @param line The line number where the error occurred.
     */
    explicit error(const char* msg, int32_t code = 0, const char* file = get_file_name(__FILE__), const char* func = "", uint32_t line = __LINE__) OSAL_NOEXCEPT;

    /**
     * @brief Constructs an error object with the given message, error type, file, function, and line number.
     *
     * @param msg The error message.
     * @param code The error type.
     * @param file The file name where the error occurred.
     * @param func The function name where the error occurred.
     * @param line The line number where the error occurred.
     */
    explicit error(const char* msg, error_type code = error_type::OS_ENO, const char* file = get_file_name(__FILE__), const char* func = "", uint32_t line = __LINE__) OSAL_NOEXCEPT
        : error(msg, static_cast<int32_t>(code), file, func, line) {}

    /**
     * @brief Constructs an error object with the given previous error, message, code, file, function, and line number.
     *
     * @param old_error The previous error.
     * @param msg The error message.
     * @param code The error code.
     * @param file The file name where the error occurred.
     * @param func The function name where the error occurred.
     * @param line The line number where the error occurred.
     */
    error(error* old_error, const char* msg, int32_t code = 0, const char* file = get_file_name(__FILE__), const char* func = "", uint32_t line = __LINE__) OSAL_NOEXCEPT;

    /**
     * @brief Constructs an error object with the given previous error, message, code, file, function, and line number.
     *
     * @param old_error The previous error.
     * @param msg The error message.
     * @param code The error code.
     * @param file The file name where the error occurred.
     * @param func The function name where the error occurred.
     * @param line The line number where the error occurred.
     */
    error(error* old_error, const char* msg, error_type code = error_type::OS_ENO, const char* file = get_file_name(__FILE__), const char* func = "", uint32_t line = __LINE__) OSAL_NOEXCEPT
    : error(old_error, msg, static_cast<int32_t>(code), file, func, line)
    {

    }

    /**
     * @brief Copy assignment operator.
     *
     * @param error The error object to be assigned.
     * @return Reference to the assigned error object.
     */
    error& operator=(const error&) = default;

    /**
     * @brief Move constructor.
     *
     * @param error The error object to be moved.
     */
    error(error&&) = default;

    /**
     * @brief Move assignment operator.
     *
     * @param error The error object to be assigned.
     * @return Reference to the assigned error object.
     */
    error& operator=(error&&) = default;

    /**
     * @brief Destructor.
     */
    virtual ~error() OSAL_NOEXCEPT;

    /**
     * @brief Adds the information from the old_error to the current error object.
     *
     * @param old_error The previous error object to be added.
     */
    void add_error(error* old_error) OSAL_NOEXCEPT;

    /**
     * @brief Returns the error message.
     *
     * @return The error message.
     */
    inline const char* get_msg() const OSAL_NOEXCEPT
    {
        return msg;
    }

    /**
     * @brief Returns the error code.
     *
     * @return The error code.
     */
    inline uint32_t get_code() const OSAL_NOEXCEPT
    {
        return code;
    }

    /**
     * @brief Returns the file name where the error occurred.
     *
     * @return The file name where the error occurred.
     */
    inline const char* get_file() const OSAL_NOEXCEPT
    {
        return file;
    }

    /**
     * @brief Returns the function name where the error occurred.
     *
     * @return The function name where the error occurred.
     */
    inline const char* get_func() const OSAL_NOEXCEPT
    {
        return func;
    }

    /**
     * @brief Returns the line number where the error occurred.
     *
     * @return The line number where the error occurred.
     */
    inline uint32_t get_line() const OSAL_NOEXCEPT
    {
        return line;
    }

    /**
     * @brief Sets the position information (file, function, and line number) where the error occurred.
     *
     * @param file The file name where the error occurred.
     * @param func The function name where the error occurred.
     * @param line The line number where the error occurred.
     */
    void set_position(const char* file = get_file_name(__FILE__), const char* func = "", uint32_t line = __LINE__) OSAL_NOEXCEPT;
};

/**
 * @brief Prints an error message with stack information.
 *
 * This function prints an error message along with stack information to the standard output.
 * It takes an rvalue reference to an error object as the first argument, a format string (fmt) as the second argument (optional),
 * and variadic arguments for formatting the error message.
 * The error message can be formatted using the format string and additional arguments.
 * The stack information is obtained from the error object.
 *
 * @tparam Args The variadic argument types.
 * @param e The rvalue reference to the error object.
 * @param fmt The format string for formatting the error message (optional).
 * @param args The additional arguments for formatting the error message.
 */
template<typename... Args>
constexpr inline void printf_stack_error(const char* app_tag, const error* e, const char* fmt = nullptr, Args... args) OSAL_NOEXCEPT
{
    printf_stack_error(app_tag, *e, fmt, args...);
}


}
}


