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

#include "osal/memory.hpp"

#include <FreeRTOS.h>
#include <stdint.h>

namespace osal
{
inline namespace v1
{

array_deleter<bool> array_deleter_bool;
array_deleter<char> array_deleter_char;
array_deleter<short> array_deleter_short;
array_deleter<int> array_deleter_int;
array_deleter<long> array_deleter_long;
array_deleter<float> array_deleter_float;
array_deleter<double> array_deleter_double;
array_deleter<int8_t> array_deleter_uint8;
array_deleter<uint8_t> array_deleter_int8;
array_deleter<int16_t> array_deleter_uint16;
array_deleter<uint16_t> array_deleter_int16;
array_deleter<int32_t> array_deleter_uint32;
array_deleter<uint32_t> array_deleter_int32;
array_deleter<int64_t> array_deleter_uint64;
array_deleter<uint64_t> array_deleter_int64;

void* malloc(size_t size)
{
	return pvPortMalloc(size);
}

void free(void* ptr)
{
	vPortFree(ptr);
}

}
}

#ifdef OSAL_MEM_LAYER
void* operator new(size_t size)
{
    return osal::malloc(size);
}

void* operator new[](size_t size)
{
    return osal::malloc(size);
}

void operator delete(void* ptr)
{
	osal::free(ptr);
}

void operator delete[](void* ptr)
{
	osal::free(ptr);
}
#endif
