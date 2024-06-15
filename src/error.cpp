/***************************************************************************
 *
 * OSAL
 * Copyright (C) 2023 / 2024  Antonio Salsi <passy.linux@zresa.it>
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
#include "osal/log.hpp"
#include <stdio.h>


namespace osal
{
inline namespace v1
{

    error::error(const char* msg, uint8_t code, const char* file, const char* func, uint32_t line) OSAL_NOEXCEPT
        : code(code)
        , line(line)
    {
        if(msg)
        {
            strncpy(&this->msg[0], msg, sizeof(this->msg));
        }
        if(file)
        {
            strncpy(&this->file[0], file, sizeof(this->file));
        }
        if(func)
        {
            strncpy(&this->func[0], func, sizeof(this->func));
        }
    }

    error::error(error* old_error, const char* msg, uint8_t code, const char* file, const char* func, uint32_t line) OSAL_NOEXCEPT
        : error(msg, code, file, func, line)
    {
        if(this->old_error)
        {
            this->old_error = nullptr;
        }
        this->old_error = old_error;
    }

    error::~error() OSAL_NOEXCEPT
    {
        if(old_error)
        {
            old_error = nullptr;
        }
    }

    void error::add_error(error* old_error) OSAL_NOEXCEPT
    {
        this->old_error = old_error;
    }

    void error::set_position(const char* file, const char* func, uint32_t line) OSAL_NOEXCEPT
    {
        if(file)
        {
            strncpy(&this->file[0], file, sizeof(this->file));
        }
        if(func)
        {
            strncpy(&this->func[0], func, sizeof(this->func));
        }
        this->line = line;
    }


    void printf_stack_error(const char* app_tag, const error &e, const char* fmt, ...) OSAL_NOEXCEPT
    {
        if(fmt && strlen(fmt))
        {
            va_list list;
            va_start (list, fmt);
            vprintf (fmt, list);
            printf ("\n");
            va_end (list);
        }

        char row[512];
        memset(row, '\0', sizeof(row));

        uint16_t count = 0;
        const error *ptr = &e;
        while(ptr)
        {
            count++;
            snprintf(row, sizeof(row), "%u. msg:%s code:%d (%s::%s line:%u)", count, ptr->msg, ptr->code, ptr->file, ptr->func, ptr->line);
            OSAL_LOG_ERROR(app_tag, "%s", row);
            ptr = ptr->old_error.get();
        }
        fflush(stdout);

    }



}
}
