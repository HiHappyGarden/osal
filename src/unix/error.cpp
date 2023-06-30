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
#include <stdio.h>


namespace osal
{
inline namespace v1
{

    inline error::error(const char *msg, uint8_t code, const char *file, const char *func, uint32_t line) OS_NOEXCEPT
        : code(code)
        , line(line)
    {
        strncpy(&this->msg[0], msg, sizeof(this->msg));
        strncpy(&this->file[0], file, sizeof(this->file));
        strncpy(&this->func[0], func, sizeof(this->func));
    }

    error::error(const error& old_error, const char* msg, uint8_t code, const char* file, const char* func, uint32_t line) OS_NOEXCEPT
        : error(msg, code, file, func, line)
    {
        if(this->old_error)
        {
            delete this->old_error;
            this->old_error = nullptr;
        }
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

    void error::add_error(const error &old_error) OS_NOEXCEPT
    {
        if(this->old_error)
        {
            delete this->old_error;
            this->old_error = nullptr;
        }
        this->old_error = new error(old_error);
    }

    void error::set_position(const char *file, const char *func, uint32_t line) OS_NOEXCEPT
    {
        strncpy(&this->file[0], file, sizeof(this->file));
        strncpy(&this->func[0], func, sizeof(this->func));
        this->line = line;
    }


    void printf_stack_error(const error &e, const char* fmt, ...)
    {
        if(fmt && strlen(fmt))
        {
            va_list list;
            va_start (list, fmt);
            vprintf (fmt, list);
            printf ("\n");
            va_end (list);
        }

        uint16_t count = 0;
        const error *ptr = &e;
        while(ptr)
        {
            count++;
            printf("%u. msg:%s code:%d (%s::%s(line: %u))\n", count, ptr->msg, ptr->code, ptr->file, ptr->func, ptr->line);
            ptr = ptr->old_error;
        }
        fflush(stdout);

    }



}
}
