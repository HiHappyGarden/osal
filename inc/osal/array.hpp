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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

namespace osal
{
inline namespace v1
{

/**
  *  @class array
  *  @brief Represents a fixed-size array with additional functionality.
  *  @tparam T The type of elements in the array.
  *  @tparam Size The fixed size of the array.
 */
template <typename T, size_t Size>
class array final
{
private:
    T data[Size]{0};
    size_t data_length{0};

public:
    /**
     * @brief Default constructor for the array class.
     */
    constexpr array() = default;

    /**
      * @brief Destructor for the array class.
      */
    inline ~array() OS_NOEXCEPT { memset(&data, 0, sizeof(data)); }

    /**
      * @brief Retrieves a pointer to the data in the array.
      * @return A pointer to the data.
      */
    constexpr inline const T* get_data() const OS_NOEXCEPT
    {
        return &data[0];
    }

    /**
      * @brief Retrieves a pointer to the data in the array.
      * @return A pointer to the data.
      */
    constexpr inline T* get_data() OS_NOEXCEPT
    {
        return &data[0];
    }


    /**
      * @brief Retrieves the size of the array.
      * @return The size of the array.
      */
    constexpr inline size_t get_size() const OS_NOEXCEPT
    {
        return Size;
    }

    /**
      * @brief Retrieves the length of the data in the array.
      * @return The length of the data in the array.
      */
    constexpr inline size_t get_length() const OS_NOEXCEPT
    {
        return data_length;
    }

    /**
      * @brief Adds an element to the end of the array.
      * @param other The element to add.
      * @return A reference to the modified array.
      */
    constexpr array<T, Size> & operator<<(const T& other) OS_NOEXCEPT
    {
        if(data_length + 1 <= get_size())
        {
            memcpy(data + data_length, &other, sizeof(T));
            data_length += 1;
        }
        return *this;
    }

    /**
      * @brief Adds an element to the end of the array.
      * @param other The element to add.
      * @return A reference to the modified array.
      */
    constexpr inline array<T, Size>& operator<<(T&& other) OS_NOEXCEPT
    {
        return (*this) << other;
    }

    /**
      * @brief Adds an element to the end of the array.
      * @param other The element to add.
      * @return A reference to the modified array.
      */
    constexpr T& operator[](size_t idx) OS_NOEXCEPT
    {
        if(idx >= data_length)
        {
            //exit(0);
        }
        return data[idx];
    }

    /**
      * @brief Adds an element to the end of the array.
      * @param other The element to add.
      * @return A reference to the modified array.
      */
    constexpr const T& operator[](size_t idx) const OS_NOEXCEPT
    {
        if(idx >= data_length)
        {
            //exit(0);
        }
        return data[idx];
    }

};

/**
   * @class array_init
   * @brief Represents a statically initialized array with additional functionality.
   * @tparam T The type of elements in the array.
   * @tparam Args The variadic template parameter pack for additional elements.
  */
template <typename T, typename... Args>
class array_init final
{
    size_t data_length{0};
    T data[sizeof... (Args) + 1]{0};

    /**

            @brief Helper function to fill the array with elements.
            @tparam TP The type of the last element.
            @param last The last element to add to the array.
         */
    template <typename TP>
    constexpr void fill(const TP& last) OS_NOEXCEPT
    {
        memcpy(&data[data_length], &last, sizeof (last));
        data_length++;
    }

    /**

            @brief Helper function to recursively fill the array with elements.
            @tparam TP The type of the first element.
            @tparam ArgsP The variadic template parameter pack for additional elements.
            @param first The first element to add to the array.
            @param args The additional elements to add to the array.
         */
    template <typename TP, typename... ArgsP>
    constexpr void fill(const TP& first, ArgsP... args) OS_NOEXCEPT
    {
        memcpy(&data[data_length], &first, sizeof (first));
        data_length++;
        fill(args...);
    }

public:

    /**

            @brief Helper function to recursively fill the array with elements.
            @tparam TP The type of the first element.
            @tparam ArgsP The variadic template parameter pack for additional elements.
            @param first The first element to add to the array.
            @param args The additional elements to add to the array.
         */

    constexpr array_init() = default;

    /**
      * @brief Destructor for the array_init class.
      */
    inline ~array_init() { memset(&data, 0, sizeof(data)); }

    /**
      * @brief Explicit constructor to initialize the array with elements.
      * @param first The first element to add to the array.
      * @param args The additional elements to add to the array.
      */
    constexpr explicit array_init(const T& first, Args... args) OS_NOEXCEPT
    {
        fill(first, args...);
    }

    /**
      * @brief Explicit constructor to initialize the array with elements.
      * @param first The first element to add to the array.
      * @param args The additional elements to add to the array.
      */
    constexpr inline const T* get_data() const OS_NOEXCEPT
    {
        return &data[0];
    }

    /**
      * @brief Explicit constructor to initialize the array with elements.
      * @param first The first element to add to the array.
      * @param args The additional elements to add to the array.
      */
    constexpr inline T* get_data() OS_NOEXCEPT
    {
        return &data[0];
    }

    /**
      * @brief Explicit constructor to initialize the array with elements.
      * @param first The first element to add to the array.
      * @param args The additional elements to add to the array.
      */
    constexpr inline size_t size() const OS_NOEXCEPT
    {
        return sizeof(data) / sizeof (data[0]);
    }

    /**
      * @brief Explicit constructor to initialize the array with elements.
      * @param first The first element to add to the array.
      * @param args The additional elements to add to the array.
      */
    constexpr inline size_t length() const OS_NOEXCEPT
    {
        return data_length;
    }

    /**
      * @brief Explicit constructor to initialize the array with elements.
      * @param first The first element to add to the array.
      * @param args The additional elements to add to the array.
      */
    constexpr array_init<T, Args...>& operator<<(const T& other) OS_NOEXCEPT
    {
        if(data_length + 1 <= size())
        {
            memcpy(data + data_length, &other, sizeof(T));
            data_length += 1;
        }
        return *this;
    }

    /**
      * @brief Explicit constructor to initialize the array with elements.
      * @param first The first element to add to the array.
      * @param args The additional elements to add to the array.
      */
    constexpr array_init<T, Args...>& operator<<(T&& other) OS_NOEXCEPT
    {
        return (*this) << other;
    }

    /**
      * @brief Adds an element to the end of the array.
      * @param other The element to add.
      * @return A reference to the modified array.
      */
    constexpr T& operator[](size_t idx) OS_NOEXCEPT
    {
        if(idx >= data_length)
        {
            //exit(0);
        }
        return data[idx];
    }

    /**
      * @brief Adds an element to the end of the array.
      * @param other The element to add.
      * @return A reference to the modified array.
      */
    constexpr const T& operator[](size_t idx) const OS_NOEXCEPT
    {
        if(idx >= data_length)
        {
            //exit(0);
        }
        return data[idx];
    }
};


}
}


