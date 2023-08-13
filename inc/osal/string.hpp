/***************************************************************************
 *
 * OSAL
 * Copyright (C) 2023  Antonio Salsi <passy.linux@zresa.it>
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
#include <stdint.h>
#include <string.h>
namespace osal
{
inline namespace v1
{

/**
 * @brief A fixed-size string class template.
 *
 * This class template provides a fixed-size string implementation with basic string manipulation functionality.
 *
 * @tparam Size The maximum size of the string, including the null-terminator.
 */
template <size_t Size = 33>
class string
{
    char data[Size]{};          ///< The character data storage.
    size_t data_length{0};      ///< The current length of the string.
    static inline char str_terminator = '\0';
public:
    /**
     * @brief Default constructor.
     */
    string() = default;

    /**
     * @brief Constructor from a C-style string literal.
     *
     * @param str The input string.
     */
    constexpr string(const char(&str)[Size]) OS_NOEXCEPT
        : string(str, Size - 1)
    {}

    /**
     * @brief Constructor from a C-style string with a specified size.
     *
     * @param str The input string.
     * @param size The size of the input string.
     */
    constexpr string(const char* str, size_t size) OS_NOEXCEPT
        : data_length(size)
    {
        if(str == nullptr)
        {
            return;
        }
        strncpy(data, str, sizeof(data));
    }

    /**
     * @brief Destructor.
     */
    inline ~string() OS_NOEXCEPT { memset(&data, '\0', sizeof(data)); }

    /**
     * @brief Returns the maximum size of the string.
     *
     * @return The maximum size of the string, excluding the null terminator.
     */
    constexpr inline size_t size() const OS_NOEXCEPT
    {
        return sizeof(data) - 1;
    }

    /**
     * @brief Returns the current length of the string.
     *
     * @return The current length of the string.
     */
    constexpr inline size_t length() const OS_NOEXCEPT
    {
        return data_length;
    }

    /**
     * @brief Returns a pointer to the C-style string representation.
     *
     * @return A pointer to the C-style string representation of the string.
     */
    constexpr inline const char* c_str() const OS_NOEXCEPT
    {
        return data;
    }

    /**
     * @brief Returns a pointer to the C-style string representation.
     *
     * @return A pointer to the C-style string representation of the string.
     */
    constexpr inline char* c_str() OS_NOEXCEPT
    {
        return data;
    }

    /**
     * @brief Concatenates the current string with a char.
     *
     * This function concatenates the current string with a char.
     *
     * @tparam SizeB The size of the character array to be concatenated.
     * @param c The character to concatenate.
     * @return A reference to the modified string.
     */
    constexpr string<Size>&operator+(char c) OS_NOEXCEPT
    {
        string<Size> ret = *this;
        if(ret.data_length + 1 <= ret.size())
        {
            ret.data[data_length] = c;
            ret.data_length++;
        }
        return ret;
    }

    /**
     * @brief Concatenates the current string with a fixed-size character array.
     *
     * This function concatenates the current string with the provided character array.
     *
     * @tparam SizeB The size of the character array to be concatenated.
     * @param b The character array to concatenate.
     * @return A reference to the modified string.
     */
    template <size_t SizeB>
    constexpr string<Size>&operator+(const char(&b)[SizeB]) OS_NOEXCEPT
    {
        string<Size> ret = *this;
        size_t size_b = strnlen(b, SizeB);
        if(ret.data_length + size_b <= ret.size())
        {
            strncpy(ret.data + ret.data_length, b, Size - 1);
            ret.data_length += size_b;
        }
        else
        {
            strncpy(ret.data + ret.data_length, b, ret.size() - length());
            ret.data_length = ret.size();
        }
        return ret;
    }

    /**
     * @brief Concatenates the current string with a character array rvalue.
     *
     * This function concatenates the current string with the provided character array.
     *
     * @tparam SizeB The size of the character array to be concatenated.
     * @param b The character array to concatenate.
     * @return A new string containing the concatenated result.
     */
    template <size_t SizeB>
    constexpr inline string<Size> operator+(char(&&b)[SizeB]) OS_NOEXCEPT
    {
        return (string<Size>{*this} + b);
    }

    /**
     * @brief Operator to concatenate a C-style string to the string.
     *
     * @param b The C-style string to concatenate.
     * @return A reference to the modified string.
     */
    constexpr string<Size>& operator+(const char* b) OS_NOEXCEPT
    {
        size_t size_b = strnlen(b, Size);
        if(data_length + size_b <= size())
        {
            strncpy(data + data_length, b, Size - 1);
            data_length += size_b;
        }
        else
        {
            strncpy(data + data_length, b, size() - length());
            data_length = size();
        }
        return *this;
    }

    /**
     * @brief Operator to concatenate a C-style string to the string (compound assignment version).
     *
     * @param b The C-style string to concatenate.
     * @return A reference to the modified string.
     */
    constexpr inline string<Size>& operator+=(const char* b) OS_NOEXCEPT
    {
        return ((*this) + b);
    }

    /**
     * @brief Appends a character array to the current string.
     *
     * This function appends the provided character array to the current string.
     *
     * @tparam SizeB The size of the character array to be appended.
     * @param b The character array to append.
     * @return A reference to the modified string.
     */
    constexpr inline string<Size>& operator+=(char c) OS_NOEXCEPT
    {
        if(data_length + 1 <= size())
        {
            data[data_length] = c;
            data_length++;
        }
        return *this;
    }

    /**
     * @brief Appends a character array to the current string.
     *
     * This function appends the provided character array to the current string.
     *
     * @tparam SizeB The size of the character array to be appended.
     * @param b The character array to append.
     * @return A reference to the modified string.
     */
    template <size_t SizeB>
    constexpr inline string<Size>& operator+=(char(&&b)[SizeB]) OS_NOEXCEPT
    {
        return ((*this) + b);
    }

    /**
     * @brief Appends a character array to the current string.
     *
     * This function appends the provided character array to the current string.
     *
     * @tparam SizeB The size of the character array to be appended.
     * @param b The character array to append.
     * @return A reference to the modified string.
     */
    template <size_t SizeB>
    constexpr inline string<Size>& operator+=(const char(&b)[SizeB]) OS_NOEXCEPT
    {
        return ((*this) + b);
    }

    /**
     * @brief Appends a character array to the current string.
     *
     * This function appends the provided character array to the current string.
     *
     * @tparam SizeB The size of the character array to be appended.
     * @param b The character array to append.
     * @return A reference to the modified string.
     */
    template <size_t SizeB>
    constexpr inline bool operator==(char(&&b)[SizeB]) OS_NOEXCEPT
    {
        return strncmp(data, b, size()) == 0;
    }

    /**
     * @brief Inequality operator with a C-style string.
     *
     * This operator checks if the current string is not equal to a C-style string.
     *
     * @param b The C-style string to compare with.
     * @return true if the strings are not equal, false otherwise.
     */
    template <size_t SizeB>
    constexpr inline bool operator==(const char(&b)[SizeB]) OS_NOEXCEPT
    {
        return strncmp(data, b, size()) == 0;
    }

    /**
     * @brief Equality operator with a C-style string.
     *
     * This operator checks if the current string is equal to a C-style string.
     *
     * @param b The C-style string to compare with.
     * @return true if the strings are equal, false otherwise.
     */
    constexpr inline bool operator==(const char* b) OS_NOEXCEPT
    {
        return strncmp(data, b, size()) == 0;
    }

    /**
     * @brief Inequality operator with a C-style string.
     *
     * This operator checks if the current string is not equal to a C-style string.
     *
     * @param b The C-style string to compare with.
     * @return true if the strings are not equal, false otherwise.
     */
    template <size_t SizeB>
    constexpr inline bool operator!=(const char(&b)[SizeB]) OS_NOEXCEPT
    {
        return strncmp(data, b, size()) != 0;
    }

    /**
     * @brief Inequality operator with a C-style string.
     *
     * This operator checks if the current string is not equal to a C-style string.
     *
     * @param b The C-style string to compare with.
     * @return true if the strings are not equal, false otherwise.
     */
    template <size_t SizeB>
    constexpr inline bool operator!=(char(&&b)[SizeB]) OS_NOEXCEPT
    {
        return strncmp(data, b, size()) != 0;
    }

    /**
     * @brief Inequality operator with a C-style string.
     *
     * This operator checks if the current string is not equal to a C-style string.
     *
     * @param b The C-style string to compare with.
     * @return true if the strings are not equal, false otherwise.
     */
    constexpr inline bool operator!=(const char* b) OS_NOEXCEPT
    {
        return strncmp(data, b, size()) != 0;
    }


    /**
     * @brief Indexing operator to access characters of the string.
     *
     * This operator allows accessing individual characters of the string by their index.
     *
     * @param idx The index of the character to access.
     * @return A pointer to the character at the specified index, or nullptr if the index is out of bounds.
     */
    constexpr char& operator[](size_t idx) OS_NOEXCEPT
    {

        if(idx >= data_length)
        {
            str_terminator = '\0';
            return str_terminator;
        }
        return data[idx];
    }

    /**
     * @brief Indexing operator to access characters of the string (const version).
     *
     * This operator allows accessing individual characters of the string by their index.
     *
     * @param idx The index of the character to access.
     * @return A pointer to the character at the specified index, or nullptr if the index is out of bounds.
     */
    constexpr const char& operator[](size_t idx) const OS_NOEXCEPT
    {

        if(idx >= data_length)
        {
            str_terminator = '\0';
            return str_terminator;
        }
        return data[idx];
    }

    /**
     * @brief Assignment operator from a C-style string.
     *
     * This operator assigns a C-style string to the current string, overwriting its contents.
     *
     * @param other The C-style string to assign.
     * @return A reference to the modified string.
     */
    constexpr string<Size>& operator=(const char* other) OS_NOEXCEPT
    {
        if(other == nullptr)
        {
            return *this;
        }
        memset(data, '\0', sizeof(data));
        data_length = 0;
        (*this) += other;
        return *this;
    }
};


}
}


