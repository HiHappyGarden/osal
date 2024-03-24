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
#include "osal/mutex.hpp"

#include <FreeRTOS.h>
#include <semphr.h>

namespace osal
{
inline namespace v1
{


mutex::mutex(class error** error) OS_NOEXCEPT : m { xSemaphoreCreateRecursiveMutex() }
{
    if(m.handle == nullptr && error)
    {
        *error = OS_ERROR_BUILD("xSemaphoreCreateRecursiveMutex() fail.", error_type::OS_EFAULT);
        OS_ERROR_PTR_SET_POSITION(*error);
    }
}

mutex::~mutex() OS_NOEXCEPT
{
    if(m.handle)
    {
        vSemaphoreDelete(m.handle);
        m.handle = nullptr;
    }
}

void mutex::lock() OS_NOEXCEPT
{
    if(m.handle)
    {
        xSemaphoreTakeRecursive(m.handle, portMAX_DELAY);
    }
}

void mutex::lock_from_isr() OS_NOEXCEPT
{
    if(m.handle)
    {
        xSemaphoreTakeFromISR(m.handle, nullptr);
    }
}

void mutex::unlock() OS_NOEXCEPT
{
    if(m.handle)
    {
        xSemaphoreGiveRecursive(m.handle);
    }
}

void mutex::unlock_from_isr() OS_NOEXCEPT
{
    if(m.handle)
    {
        xSemaphoreGiveFromISR(m.handle, nullptr);
    }
}

}
}
