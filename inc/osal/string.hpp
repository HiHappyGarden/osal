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
#include "osal/iterator.hpp"

#include <stdint.h>
#include <string.h>
#include <limits.h>

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
class string final
{
    char data[Size]{};          ///< The character data storage.
    mutable size_t data_length{0};      ///< The current length of the string.

    static inline char terminator = '\0'; //keep non constexpr
public:

    using iterator =  osal::iterator<char>;
    using const_iterator = osal::iterator<char>;



    /**
     * @brief Default constructor.
     */
    string() = default;

    /**
     * @brief Constructor from a C-style string literal.
     *
     * @param str The input string.
     */
    constexpr string(const char(&str)[Size]) OS_NOEXCEPT //keep not explicit
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
    	data_length = strlen(data);
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

    constexpr inline void clear() OS_NOEXCEPT
    {
        memset(&data, '\0', sizeof(data));
        data_length = 0;
    }

    constexpr inline auto begin() OS_NOEXCEPT { return data; }
    constexpr inline auto end() OS_NOEXCEPT { return data + data_length; }

    constexpr inline auto begin() const OS_NOEXCEPT { return data; }
    constexpr inline auto end() const OS_NOEXCEPT { return data + data_length; }

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
        data_length = strlen(data);
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
        data_length = strlen(data);
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
        size_t size_b = strlen(b);
        data_length = strlen(data);
        if(size_b > Size)
        {
        	size_b = Size;
        }
        if(data_length + size_b <= size())
        {
            strncpy(data + data_length, b, size_b);
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
    	data_length = strlen(data);
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
    	data_length = strlen(data);
        if(idx >= data_length)
        {
            terminator = '\0';
            return terminator;
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
    	data_length = strlen(data);
        if(idx >= data_length)
        {
            terminator = '\0';
            return terminator;
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

    inline const char* find(const char to_find[], size_t offset = 0) const OS_NOEXCEPT
    {
        return ::strstr(&data[offset], to_find);
    }

    inline bool start_with(const char to_start[], size_t offset = 0) const OS_NOEXCEPT
    {
        return strncmp(&data[offset], to_start, strlen(to_start)) == 0;
    }

    string<Size>& ltrim() OS_NOEXCEPT
    {
    	data_length = strlen(data);
        size_t count = 0;
        for(auto ch : data)
        {
            if(!isspace(ch))
            {
                break;
            }
            count++;
        }

        memmove(data, data + count, data_length - count);
        data_length -= count;
        memset(data + data_length, '\0', sizeof(data) - data_length);

        return *this;
    }

    string<Size>& rtrim() OS_NOEXCEPT
    {
    	data_length = strlen(data);
        size_t count = 0;
        for(size_t i = data_length - 1; i != 0; i--)
        {
            if(!isspace(data[i]))
            {
                break;
            }
            count++;
        }

        data[data_length - count] = '\0';
        data_length -= count;
        memset(data + data_length, '\0', sizeof(data) - data_length);
        return *this;
    }

    string<Size> strstr(size_t idx_start, size_t offset = SIZE_MAX) const OS_NOEXCEPT
    {
    	data_length = strlen(data);
    	if(idx_start >= data_length)
    	{
    		return {};
    	}

    	if(idx_start + offset >= data_length)
    	{
    		if(data_length) {
    			offset = data_length - idx_start - 1;
    		}
    		else
    		{
    			offset = 0;
    		}
    	}

    	string<Size> ret;
    	for(size_t i = idx_start; i < idx_start + offset; i++)
    	{
    		ret += data[i];
    	}

    	return ret;
    }

    char * replace(char const * const pattern,  char const * const replacement) const OS_NOEXCEPT
    {


        return nullptr;
    }

};


}
}


