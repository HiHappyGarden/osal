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
#include "osal/semaphore.hpp"


namespace osal
{
inline namespace v1
{

semaphore::semaphore(size_t count, error** error) OS_NOEXCEPT : sem { xSemaphoreCreateCounting (UINT32_MAX, count) }
{
    if(sem.handle == nullptr && error)
    {
        *error = OS_ERROR_BUILD("xSemaphoreCreateCounting() fail.", error_type::OS_EFAULT);
        OS_ERROR_PTR_SET_POSITION(*error);
    }
}

semaphore::~semaphore()
{
    if(sem.handle)
    {
        vSemaphoreDelete(sem.handle);
        sem.handle = nullptr;
    }
}

osal::exit semaphore::wait(uint64_t time, error** error) OS_NOEXCEPT
{
    if(sem.handle == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("xSemaphoreCreateCounting() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    if (xSemaphoreTake (sem.handle, tmo_to_ticks(time)) == pdTRUE)
    {
        return exit::OK;
    }

    return exit::KO;
}

osal::exit semaphore::wait_from_isr(uint64_t time, error **error) OS_NOEXCEPT
{
    if(sem.handle == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("xSemaphoreCreateCounting() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    if (xSemaphoreTakeFromISR (sem.handle, nullptr) == pdTRUE)
    {
        return exit::OK;
    }

    return exit::KO;
}

void semaphore::signal() OS_NOEXCEPT
{
    if(sem.handle)
    {
        xSemaphoreGive(sem.handle);
    }
}

void semaphore::signal_from_isr() OS_NOEXCEPT
{
    if(sem.handle)
    {
        xSemaphoreGiveFromISR(sem.handle, nullptr);
    }
}


}
}
