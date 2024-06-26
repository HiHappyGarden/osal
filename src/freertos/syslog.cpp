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
#include <sys/timeb.h>

namespace osal
{
inline namespace v1
{

uint64_t system_current_time_millis() OS_NOEXCEPT
{
#if defined(_WIN32) || defined(_WIN64)
    struct _timeb timebuffer;
    _ftime(&timebuffer);
    return (uint64_t)(((timebuffer.time * 1000) + timebuffer.millitm));
#elif defined(__UNIX__)
    struct timeb timebuffer;
    ftime(&timebuffer);
    return (uint64_t)(((timebuffer.time * 1000) + timebuffer.millitm));
#else
    return 0;
#endif
}
extern void us_sleep(uint64_t us) OS_NOEXCEPT;

extern uint8_t log_level;

namespace
{
    bool busy = false;
}

void sys_log(const char* tag, uint8_t type, const char* fmt, ...) OS_NOEXCEPT
{
    if(busy)
    {
        while (busy)
        {
            us_sleep(1'000); // 1ms
        }
    }
    busy = true;
	va_list list;
	bool enable_print = false;

	if( !(log_level & FLAG_STATE_ON) )
	{
        busy = false;
		return;
	}

	uint32_t milli_sec = 0;
	if(::osal_system_current_time_millis)
	{
		milli_sec = ::osal_system_current_time_millis();
	}
	else
	{
		milli_sec = system_current_time_millis();
	}


	switch (get_level_log(type))
	{
        case FLAG_DEBUG:
            if(log_level & FLAG_DEBUG)
			{
				enable_print = true;
				OS_LOG_PRINTF (OS_ANSI_COLOR_CYAN "%lu|DEBUG|%s: ", milli_sec, tag);
			}
			break;
        case FLAG_INFO:
            if(log_level & FLAG_INFO)
			{
				enable_print = true;
				OS_LOG_PRINTF (OS_ANSI_COLOR_GREEN "%lu|INFO |%s: ", milli_sec, tag);
			}
			break;
        case FLAG_WARNING:
            if(log_level & FLAG_WARNING)
			{
				enable_print = true;
				OS_LOG_PRINTF (OS_ANSI_COLOR_YELLOW "%lu|WARN |%s: ", milli_sec, tag);
			}
			break;
        case FLAG_ERROR:
            if(log_level & FLAG_ERROR)
			{
				enable_print = true;
				OS_LOG_PRINTF (OS_ANSI_COLOR_RED "%lu|ERROR|%s: ", milli_sec, tag);
			}
			break;
        case FLAG_FATAL:
            if(log_level & FLAG_FATAL)
			{
				enable_print = true;
				OS_LOG_PRINTF (OS_ANSI_COLOR_MAGENTA "%lu|FATAL|%s: ", milli_sec, tag);
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
    busy = false;
}

}
}
