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

namespace osal
{
inline namespace v1
{
    uint8_t log_level = FLAG_STATE_ON | LEVEL_DEBUG;

    void set_level_log(uint8_t t) OSAL_NOEXCEPT
    {
        log_level = (log_level &  FLAG_STATE_ON) | (t & LEVEL_MASK);
    }

    void set_enable_log(bool t) OSAL_NOEXCEPT
    {
        if(t)
        {
            log_level |= FLAG_STATE_ON;
        }
        else
        {
            log_level &= ~(FLAG_STATE_ON);
        }
    }

    bool get_enable_log() OSAL_NOEXCEPT
    {
        return (log_level & FLAG_STATE_ON) > 0;
    }

    bool is_enabled_log(uint8_t type) OSAL_NOEXCEPT
    {
        return (get_level_log(type) >= log_level) && (type & FLAG_STATE_ON);
    }

}
}
