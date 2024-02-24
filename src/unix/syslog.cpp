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

#include "osal/log.hpp"

#include <math.h>
#include <time.h>
#include <sys/time.h>

namespace osal
{
    inline namespace v1
    {

        extern uint8_t log_level;

        void sys_log(const char* tag, uint8_t type, const char* fmt, ...) OS_NOEXCEPT
        {
            va_list list;
            time_t  raw_time{0};
            tm      time_struct{0};
            timeval tv{0};
            char     timestamp[26]{0};
            uint32_t milli_sec{0};
            bool     enable_print = false;

            if( !(log_level & FLAG_STATE_ON) )
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
                case FLAG_DEBUG:
                    if(log_level & FLAG_DEBUG)
                    {
                        enable_print = true;
                        OS_LOG_PRINTF (OS_ANSI_COLOR_CYAN "%s.%03d %s - DEBUG: ", timestamp, milli_sec, tag);
                    }
                    break;
                case FLAG_INFO:
                    if(log_level & FLAG_INFO)
                    {
                        enable_print = true;
                        OS_LOG_PRINTF (OS_ANSI_COLOR_GREEN "%s.%03d %s - INFO : ", timestamp, milli_sec, tag);
                    }
                    break;
                case FLAG_WARNING:
                    if(log_level & FLAG_WARNING)
                    {
                        enable_print = true;
                        OS_LOG_PRINTF (OS_ANSI_COLOR_YELLOW "%s.%03d %s - WARN : ", timestamp, milli_sec, tag);
                    }
                    break;
                case FLAG_ERROR:
                    if(log_level & FLAG_ERROR)
                    {
                        enable_print = true;
                        OS_LOG_PRINTF (OS_ANSI_COLOR_RED "%s.%03d %s - ERROR: ", timestamp, milli_sec, tag);
                    }
                    break;
                case FLAG_FATAL:
                    if(log_level & FLAG_FATAL)
                    {
                        enable_print = true;
                        OS_LOG_PRINTF (OS_ANSI_COLOR_MAGENTA "%s.%03d %s - FATAL: ", timestamp, milli_sec, tag);
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
            	OS_LOG_PRINTF(OS_ANSI_COLOR_RESET OS_LOG_NEW_LINE);
                fflush (stdout);
            }
        }

    }
}
