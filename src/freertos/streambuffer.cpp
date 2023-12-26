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
#include "osal/streambuffer.hpp"

namespace osal
{
inline namespace v1
{

stream_buffer::stream_buffer(size_t size, size_t trigger_size, error** error) OS_NOEXCEPT
: sb { xStreamBufferCreate(size, trigger_size) }
{
    if(sb.handle == nullptr && error)
    {
        *error = OS_ERROR_BUILD("xStreamBufferCreate() fail.", error_type::OS_EFAULT);
        OS_ERROR_PTR_SET_POSITION(*error);
    }
}

stream_buffer::~stream_buffer()
{
    if(sb.handle)
    {
        vStreamBufferDelete(sb.handle);
        sb.handle = nullptr;
    }
}

size_t stream_buffer::send(const uint8_t *data, size_t size, uint64_t time, error** error) OS_NOEXCEPT
{
    if(sb.handle == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("xStreamBufferCreate() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return 0;
    }

    return xStreamBufferSend(sb.handle, data, size, tmo_to_ticks(time));
}

size_t stream_buffer::send_from_isr(const uint8_t *data, size_t size, uint64_t time, error **error) OS_NOEXCEPT
{
    if(sb.handle == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("xStreamBufferCreate() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return 0;
    }

    BaseType_t higher_priority_task_woken = pdFALSE;
    size_t ret = xStreamBufferSendFromISR(sb.handle, data, size, &higher_priority_task_woken);
    portYIELD_FROM_ISR(pdFALSE);
    return ret;
}

size_t stream_buffer::receive(uint8_t *data, size_t size, uint64_t time, error **error) OS_NOEXCEPT
{
    if(sb.handle == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("xStreamBufferCreate() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return 0;
    }

    return xStreamBufferReceive(sb.handle, data, size, tmo_to_ticks(time));
}

size_t stream_buffer::receive_from_isr(uint8_t *data, size_t size, uint64_t time, error **error) OS_NOEXCEPT
{
    if(sb.handle == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("xStreamBufferCreate() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return 0;
    }

    BaseType_t higher_priority_task_woken = pdFALSE;
    size_t ret = xStreamBufferReceiveFromISR(sb.handle, data, size, &higher_priority_task_woken);
    portYIELD_FROM_ISR(pdFALSE);

    return ret;
}

void stream_buffer::reset() OS_NOEXCEPT
{
    if(sb.handle)
    {
        xStreamBufferReset(sb.handle);
    }
}

bool stream_buffer::is_empty() const OS_NOEXCEPT
{
    if(sb.handle)
    {
        return xStreamBufferIsEmpty(sb.handle) == pdTRUE;
    }
    return false;
}

bool stream_buffer::is_full() const OS_NOEXCEPT
{
    if(sb.handle)
    {
        return xStreamBufferIsFull(sb.handle) == pdTRUE;
    }
    return false;
}

size_t stream_buffer::size() const OS_NOEXCEPT
{
    if(sb.handle)
    {
        return xStreamBufferBytesAvailable(sb.handle);
    }
    return 0;
}

size_t stream_buffer::bytes_free() const
{
    if(sb.handle)
    {
        return xStreamBufferSpacesAvailable(sb.handle);
    }
    return 0;
}

}
}
