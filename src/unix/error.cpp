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
#include "osal/error.hpp"

namespace osal
{
inline namespace v1
{

    namespace
    {
        error last_error;
    }


    error::error(const char *msg, uint8_t code, const char *file, const char *func, uint32_t line) OS_NOEXCEPT
        : msg(msg)
        , code(code)
        , file(file)
        , func(func)
        , line(line)
    {

        last_error = *this;
    }

    error::error(const error& old_error, const char* msg, uint8_t code, const char* file, const char* func, uint32_t line) OS_NOEXCEPT
        : error(msg, code, file, func, line)
    {
        this->old_error = new error(old_error);
    }


    error::~error() OS_NOEXCEPT
    {
        if(old_error)
        {
            delete old_error;
            old_error = nullptr;
        }
    }


}
}
