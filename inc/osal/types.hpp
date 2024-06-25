/***************************************************************************
 *
 * OSAL
 * Copyright (C) 2023/2024 Antonio Salsi <passy.linux@zresa.it>
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

#pragma once

#include <stdlib.h>
#include <stdint.h>


#ifndef RTTI_ENABLED
    #ifndef dynamic_cast
        #define dynamic_cast reinterpret_cast
    #endif
#endif

#ifdef __cpp_exceptions
    #ifndef OSAL_NOEXCEPT
        #define OSAL_NOEXCEPT noexcept
    #endif
#else
    #ifndef OSAL_NOEXCEPT
        #define OSAL_NOEXCEPT
    #endif
#endif

#define OSAL_TO_STR(str) #str

namespace osal
{
inline namespace v1
{

struct true_type { static constexpr inline const bool value = true; };

struct false_type { static constexpr inline const bool value = false; };


/**
 * @brief Type definition for the null pointer constant.
 *
 * The nullptr_t type is used to represent the type of the nullptr literal.
 * It is a distinct type that can be used for overloading purposes or to explicitly specify a null pointer value.
 */
using nullptr_t = decltype(nullptr);


enum class error_type : int32_t
{
    OS_ENO =  0, /* No error */
    OS_EPERM =  1, /* Operation not permitted */
    OS_ENOENT =  2, /* No such file or directory */
    OS_ESRCH =  3, /* No such process */
    OS_EINTR =  4, /* Interrupted system call */
    OS_EIO =  5, /* I/O error */
    OS_ENXIO =  6, /* No such device or address */
    OS_E2BIG =  7, /* Argument list too long */
    OS_ENOEXEC =  8, /* Exec format error */
    OS_EBADF =  9, /* Bad file number */
    OS_ECHILD = 10, /* No child processes */
    OS_EAGAIN = 11, /* Try again */
    OS_ENOMEM = 12, /* Out of memory */
    OS_EACCES = 13, /* Permission denied */
    OS_EFAULT = 14, /* Bad address */
    OS_ENOTBLK = 15, /* Block device required */
    OS_EBUSY = 16, /* Device or resource busy */
    OS_EEXIST = 17, /* File exists */
    OS_EXDEV = 18, /* Cross-device link */
    OS_ENODEV = 19, /* No such device */
    OS_ENOTDIR = 20, /* Not a directory */
    OS_EISDIR = 21, /* Is a directory */
    OS_EINVAL = 22, /* Invalid argument */
    OS_ENFILE = 23, /* File table overflow */
    OS_EMFILE = 24, /* Too many open files */
    OS_ENOTTY = 25, /* Not a typewriter */
    OS_ETXTBSY = 26, /* Text file busy */
    OS_EFBIG = 27, /* File too large */
    OS_ENOSPC = 28, /* No space left on device */
    OS_ESPIPE = 29, /* Illegal seek */
    OS_EROFS = 30, /* Read-only file system */
    OS_EMLINK = 31, /* Too many links */
    OS_EPIPE = 32, /* Broken pipe */
    OS_EDOM = 33, /* Math argument out of domain of func */
    OS_ERANGE = 34, /* Math result not representable */
    OS_EDEADLK = 35, /* Resource deadlock would occur */
    OS_ENAMETOOLONG = 36, /* File name too long */
    OS_ENOLCK = 37, /* No record locks available */

/*
* This error code is special: arch syscall entry code will return
* -ENOSYS if users try to call a syscall that doesn't exist.  To keep
* failures of syscalls that really do exist distinguishable from
* failures due to attempts to use a nonexistent syscall, syscall
* implementations should refrain from returning -ENOSYS.
*/
    OS_ENOSYS = 38, /* Invalid system call number */

    OS_ENOTEMPTY = 39, /* Directory not empty */
    OS_ELOOP = 40, /* Too many symbolic links encountered */
    OS_EWOULDBLOCK = OS_EAGAIN, /* Operation would block */
    OS_ENOMSG = 42, /* No message of desired type */
    OS_EIDRM = 43, /* Identifier removed */
    OS_ECHRNG = 44, /* Channel number out of range */
    OS_EL2NSYNC = 45, /* Level 2 not synchronized */
    OS_EL3HLT = 46, /* Level 3 halted */
    OS_EL3RST = 47, /* Level 3 reset */
    OS_ELNRNG = 48, /* Link number out of range */
    OS_EUNATCH = 49, /* Protocol driver not attached */
    OS_ENOCSI = 50, /* No CSI structure available */
    OS_EL2HLT = 51, /* Level 2 halted */
    OS_EBADE = 52, /* Invalid exchange */
    OS_EBADR = 53, /* Invalid request descriptor */
    OS_EXFULL = 54, /* Exchange full */
    OS_ENOANO = 55, /* No anode */
    OS_EBADRQC = 56, /* Invalid request code */
    OS_EBADSLT = 57, /* Invalid slot */

    OS_EDEADLOCK = OS_EDEADLK,

    OS_EBFONT = 59, /* Bad font file format */
    OS_ENOSTR = 60, /* Device not a stream */
    OS_ENODATA = 61, /* No data available */
    OS_ETIME = 62, /* Timer expired */
    OS_ENOSR = 63, /* Out of streams resources */
    OS_ENONET = 64, /* Machine is not on the network */
    OS_ENOPKG = 65, /* Package not installed */
    OS_EREMOTE = 66, /* Object is remote */
    OS_ENOLINK = 67, /* Link has been severed */
    OS_EADV = 68, /* Advertise error */
    OS_ESRMNT = 69, /* Srmount error */
    OS_ECOMM = 70, /* Communication error on send */
    OS_EPROTO = 71, /* Protocol error */
    OS_EMULTIHOP = 72, /* Multihop attempted */
    OS_EDOTDOT = 73, /* RFS specific error */
    OS_EBADMSG = 74, /* Not a data message */
    OS_EOVERFLOW = 75, /* Value too large for defined data type */
    OS_ENOTUNIQ = 76, /* Name not unique on network */
    OS_EBADFD = 77, /* File descriptor in bad state */
    OS_EREMCHG = 78, /* Remote address changed */
    OS_ELIBACC = 79, /* Can not access a needed shared library */
    OS_ELIBBAD = 80, /* Accessing a corrupted shared library */
    OS_ELIBSCN = 81, /* .lib section in a.out corrupted */
    OS_ELIBMAX = 82, /* Attempting to link in too many shared libraries */
    OS_ELIBEXEC = 83, /* Cannot exec a shared library directly */
    OS_EILSEQ = 84, /* Illegal byte sequence */
    OS_ERESTART = 85, /* Interrupted system call should be restarted */
    OS_ESTRPIPE = 86, /* Streams pipe error */
    OS_EUSERS = 87, /* Too many users */
    OS_ENOTSOCK = 88, /* Socket operation on non-socket */
    OS_EDESTADDRREQ = 89, /* Destination address required */
    OS_EMSGSIZE = 90, /* Message too long */
    OS_EPROTOTYPE = 91, /* Protocol wrong type for socket */
    OS_ENOPROTOOPT = 92, /* Protocol not available */
    OS_EPROTONOSUPPORT = 93, /* Protocol not supported */
    OS_ESOCKTNOSUPPORT = 94, /* Socket type not supported */
    OS_EOPNOTSUPP = 95, /* Operation not supported on transport endpoint */
    OS_EPFNOSUPPORT = 96, /* Protocol family not supported */
    OS_EAFNOSUPPORT = 97, /* Address family not supported by protocol */
    OS_EADDRINUSE = 98, /* Address already in use */
    OS_EADDRNOTAVAIL = 99, /* Cannot assign requested address */
    OS_ENETDOWN = 100, /* Network is down */
    OS_ENETUNREACH = 101, /* Network is unreachable */
    OS_ENETRESET = 102, /* Network dropped connection because of reset */
    OS_ECONNABORTED = 103, /* Software caused connection abort */
    OS_ECONNRESET = 104, /* Connection reset by peer */
    OS_ENOBUFS = 105, /* No buffer space available */
    OS_EISCONN = 106, /* Transport endpoint is already connected */
    OS_ENOTCONN = 107, /* Transport endpoint is not connected */
    OS_ESHUTDOWN = 108, /* Cannot send after transport endpoint shutdown */
    OS_ETOOMANYREFS = 109, /* Too many references: cannot splice */
    OS_ETIMEDOUT = 110, /* Connection timed out */
    OS_ECONNREFUSED = 111, /* Connection refused */
    OS_EHOSTDOWN = 112, /* Host is down */
    OS_EHOSTUNREACH = 113, /* No route to host */
    OS_EALREADY = 114, /* Operation already in progress */
    OS_EINPROGRESS = 115, /* Operation now in progress */
    OS_ESTALE = 116, /* Stale file handle */
    OS_EUCLEAN = 117, /* Structure needs cleaning */
    OS_ENOTNAM = 118, /* Not a XENIX named type file */
    OS_ENAVAIL = 119, /* No XENIX semaphores available */
    OS_EISNAM = 120, /* Is a named type file */
    OS_EREMOTEIO = 121, /* Remote I/O error */
    OS_EDQUOT = 122, /* Quota exceeded */

    OS_ENOMEDIUM = 123, /* No medium found */
    OS_EMEDIUMTYPE = 124, /* Wrong medium type */
    OS_ECANCELED = 125, /* Operation Canceled */
    OS_ENOKEY = 126, /* Required key not available */
    OS_EKEYEXPIRED = 127, /* Key has expired */
    OS_EKEYREVOKED = 128, /* Key has been revoked */
    OS_EKEYREJECTED = 129, /* Key was rejected by service */

    /* for robust mutexes */
    OS_EOWNERDEAD = 130, /* Owner died */
    OS_ENOTRECOVERABLE = 131, /* State not recoverable */

    OS_ERFKILL = 132, /* Operation not possible due to RF-kill */

    OS_EHWPOISON = 133, /* Memory page has hardware error */

    OS_OUTRNG = 135, /* Out of range*/
    OS_CASTERR = 136, /* Cast error*/
    OS_VALCONV = 137, /* Value conversion error */
	OS_ERCRC = 138, /* Crc error */
    OS_EXCMAXVAL = 138, /* Exceed max values permitted */
};


constexpr const uint64_t USECS_PER_SEC = 1'000'000;
constexpr const uint64_t NSECS_PER_SEC = 1'000'000'000;

constexpr const uint64_t WAIT_FOREVER = 0xFFFFFFFFFFFFFFFF;

enum class exit : uint8_t
{
    OK = EXIT_SUCCESS,
    KO = EXIT_FAILURE
};

template<typename T, typename V>
struct pair final
{
	T t;
	V v;
};

}
}



