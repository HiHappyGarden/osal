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
#include "osal/log.hpp"

#include <math.h>
#include <time.h>
#include <sys/time.h>


namespace osal
{
inline namespace v1
{
    uint8_t log_level = LEVEL_DEBUG | LOG_STATE_ON;

    void set_level_log(uint8_t t) OS_NOEXCEPT
    {
        log_level |= (t & LEVEL_MASK);
    }

    void set_enable_log(bool t) OS_NOEXCEPT
    {
        if(t)
        {
            log_level &= LOG_STATE_ON;
        }
        else
        {
            log_level &= ~(LOG_STATE_ON);
        }
    }

    bool get_enable_log() OS_NOEXCEPT
    {
        return (log_level & LOG_STATE_ON) > 0;
    }

    bool is_enabled_log(uint8_t type) OS_NOEXCEPT
    {
        return (get_level_log(type) >= log_level) && (type & LOG_STATE_ON);
    }

    void log(const char* tag, uint8_t type, const char* fmt, ...) OS_NOEXCEPT
    {
        va_list list;
        time_t  raw_time{0};
        tm      time_struct{0};
        timeval tv{0};
        char     timestamp[26]{0};
        uint32_t milli_sec{0};
        bool     enable_print = false;

        if( !(log_level & LOG_STATE_ON) )
        {
            return;
        }

        gettimeofday(&tv, nullptr);

        milli_sec = lrint(tv.tv_usec / 1'000l); // Round to nearest milli_sec

        time (&raw_time);
        localtime_r (&raw_time, &time_struct);
        strftime (timestamp, sizeof (timestamp), "%H:%M:%S", &time_struct);

        switch (get_level_log(type))
        {
        case LEVEL_DEBUG:
            if(log_level & (LEVEL_DEBUG))
            {
                enable_print = true;
                printf (OS_ANSI_COLOR_CYAN "%s.%03d %s - DEBUG: ", timestamp, milli_sec, tag);
            }
            break;
        case LEVEL_INFO:
            if(log_level & (LEVEL_DEBUG|LEVEL_INFO))
            {
                enable_print = true;
                printf (OS_ANSI_COLOR_GREEN "%s.%03d %s - INFO : ", timestamp, milli_sec, tag);
            }
            break;
        case LEVEL_WARNING:
            if(log_level & (LEVEL_DEBUG|LEVEL_INFO|LEVEL_WARNING))
            {
                enable_print = true;
                printf (OS_ANSI_COLOR_YELLOW "%s.%03d %s - WARN : ", timestamp, milli_sec, tag);
            }
            break;
        case LEVEL_ERROR:
            if(log_level & (LEVEL_DEBUG|LEVEL_INFO|LEVEL_WARNING|LEVEL_ERROR))
            {
                enable_print = true;
                printf (OS_ANSI_COLOR_RED "%s.%03d %s - ERROR: ", timestamp, milli_sec, tag);
            }
            break;
        case LEVEL_FATAL:
            if(log_level & (LEVEL_DEBUG|LEVEL_INFO|LEVEL_WARNING|LEVEL_ERROR|LEVEL_FATAL))
            {
                enable_print = true;
                printf (OS_ANSI_COLOR_MAGENTA "%s.%03d %s - FATAL: ", timestamp, milli_sec, tag);
            }
            break;
        default:
            enable_print = false;
            break;
        }

        va_start (list, fmt);
        if(enable_print)
            vprintf (fmt, list);
        va_end (list);
        if(enable_print)
        {
            printf("\n");
            fflush (stdout);
        }
    }

}
}
