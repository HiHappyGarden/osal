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

namespace osal
{
inline namespace v1
{
    uint8_t log_level = LEVEL_DEBUG | STATE_ON;

    inline void set_log_level(uint8_t t) OS_NOEXCEPT
    {
        log_level |= t;
    }

    inline void set_enable_log(bool t) OS_NOEXCEPT
    {
        if(t)
        {
            log_level &= STATE_ON;
        }
        else
        {
            log_level &= ~(STATE_ON);
        }
    }

    inline bool get_enable_log() OS_NOEXCEPT
    {
        return (log_level & STATE_ON) > 0;
    }

    inline bool is_enabled_log(uint8_t type)
    {
        return (get_level_log(type) >= log_level) && (type & STATE_ON);
    }

    void log(const char *tag, const char *format, ...)
    {

    }

}
}
