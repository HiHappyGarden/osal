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


extern uint8_t log_level;

void sys_log(const char* tag, uint8_t type, const char* fmt, ...) OS_NOEXCEPT
{
	va_list list;
	bool enable_print = false;

	uint32_t milli_sec = 0;
	if(::osal_system_current_time_millis)
	{
		milli_sec = ::osal_system_current_time_millis();
	}
	else
	{
		milli_sec = system_current_time_millis();
	}

	if( !(log_level & LOG_STATE_ON) )
	{
		return;
	}

	switch (get_level_log(type))
	{
		case LEVEL_DEBUG:
			if(log_level & (LEVEL_DEBUG))
			{
				enable_print = true;
				OS_LOG_PRINTF (OS_ANSI_COLOR_CYAN "%lu|DEBUG|%s: ", milli_sec, tag);
			}
			break;
		case LEVEL_INFO:
			if(log_level & (LEVEL_DEBUG|LEVEL_INFO))
			{
				enable_print = true;
				OS_LOG_PRINTF (OS_ANSI_COLOR_GREEN "%lu|INFO |%s: ", milli_sec, tag);
			}
			break;
		case LEVEL_WARNING:
			if(log_level & (LEVEL_DEBUG|LEVEL_INFO|LEVEL_WARNING))
			{
				enable_print = true;
				OS_LOG_PRINTF (OS_ANSI_COLOR_YELLOW "%lu|WARN |%s: ", milli_sec, tag);
			}
			break;
		case LEVEL_ERROR:
			if(log_level & (LEVEL_DEBUG|LEVEL_INFO|LEVEL_WARNING|LEVEL_ERROR))
			{
				enable_print = true;
				OS_LOG_PRINTF (OS_ANSI_COLOR_RED "%lu|ERROR|%s: ", milli_sec, tag);
			}
			break;
		case LEVEL_FATAL:
			if(log_level & (LEVEL_DEBUG|LEVEL_INFO|LEVEL_WARNING|LEVEL_ERROR|LEVEL_FATAL))
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
		OS_LOG_PRINTF(OS_ANSI_COLOR_RESET "\r\n");
		fflush (stdout);
	}
}

}
}
