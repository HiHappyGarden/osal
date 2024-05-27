cmake_minimum_required(VERSION 3.16...3.24)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

PROJECT(osal VERSION 1.0.0 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
set(OS_HAVE_CONFIG ON)

add_definitions(-DINCLUDE_OS_CONFIG) 

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

# Default settings if this is the main project
if (CMAKE_PROJECT_NAME STREQUAL osal)
    message(STATUS "set default settings")
    include(CTest)

    # Default to release build with debug info
    if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
    endif (NOT CMAKE_BUILD_TYPE)

    # Default to installing in build directory
    if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${OSAL_BINARY_DIR}/install
      CACHE PATH "Default install path" FORCE)
    endif()

    message(STATUS "Current build type is: ${CMAKE_BUILD_TYPE}")
    message(STATUS "Current install path is: ${CMAKE_INSTALL_PREFIX}")
    message(STATUS "Building for ${CMAKE_SYSTEM_NAME}")
endif()

set(CMAKE_C_OUTPUT_EXTENSION_REPLACE 1)
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)




#foreach(i ${CMAKE_CXX_COMPILE_FEATURES})
#  #message("-- ${i}")
#  if( ${i} STREQUAL "cxx_noexcept")
#      message(STATUS "Find noexcept")
#      add_definitions(-DOS_NOEXCEPT=noexcept)
#  endif()
#endforeach()

if (CMAKE_PROJECT_NAME STREQUAL osal AND ENABLE_TEST)
    file(GLOB_RECURSE OSAL_TEST CONFIGURE_DEPENDS "test/*.cpp")

    include(FetchContent)
    message(STATUS "Start googletest download")
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

include_directories(inc)
include_directories(${PROJECT_SOURCE_DIR}/config)

if(UNIX)
    add_compile_options("$<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-fno-rtti>")
    add_compile_options("$<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-fno-exceptions>")

    if (ENABLE_FREERTOS)
        message(STATUS "OSAL for FreeRTOS linux emulation")

        message(STATUS "Start freertos download")
        FetchContent_Declare( freertos_kernel
                GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Kernel.git
                GIT_TAG        V11.0.1
        )

        add_library(freertos_config INTERFACE)

        target_include_directories(freertos_config SYSTEM
                INTERFACE
                src/freertos/config
        )

        target_compile_definitions(freertos_config
                INTERFACE
                projCOVERAGE_TEST=0
        )

        set( FREERTOS_HEAP "4" CACHE STRING "" FORCE)
        # Select the native compile PORT
        set( FREERTOS_PORT "GCC_POSIX" CACHE STRING "" FORCE)
        # Select the cross-compile PORT
#        if (CMAKE_CROSSCOMPILING)
#            set(FREERTOS_PORT "GCC_ARM_CA9" CACHE STRING "" FORCE)
#        endif()
        include_directories(${CMAKE_BINARY_DIR}/_deps/freertos_kernel-src/include)
        include_directories(${CMAKE_BINARY_DIR}/_deps/freertos_kernel-src/portable/ThirdParty/GCC/Posix)

        set(OS_MEM_LAYER 1)
        set(PLATFORM_LIB "freertos_kernel")
        FetchContent_MakeAvailable(freertos_kernel)
        set(PLATFORM_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/freertos ${CMAKE_CURRENT_SOURCE_DIR}/src/freertos/config)
        include_directories(${PLATFORM_INCLUDE})
        file(GLOB_RECURSE OSAL_INCLUDES CONFIGURE_DEPENDS "inc/*.hpp" "src/freertos/osal_sys/*.hpp" "src/freertos/config/*.h")
        file(GLOB_RECURSE OSAL_SOURCES CONFIGURE_DEPENDS "src/error.cpp" "src/log.cpp" "src/generics.cpp" "src/freertos/*.cpp")
    else()
        message(STATUS "OSAL for LINUX")
        set(PLATFORM_LIB dl)
        set(PLATFORM_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/unix)
        include_directories(${PLATFORM_INCLUDE})
        file(GLOB_RECURSE OSAL_INCLUDES CONFIGURE_DEPENDS "inc/*.hpp" "src/unix/osal_sys/*.hpp")
        file(GLOB_RECURSE OSAL_SOURCES CONFIGURE_DEPENDS "src/error.cpp" "src/log.cpp" "src/generics.cpp" "src/unix/*.cpp")

    endif()

    set(LOG_NEW_LINE \\n)
elseif (WIN32)
    message(STATUS "OSAL for FreeRTOS win32 emulation")

    option(OS_MEM_LAYER "Enable FreeRTOS wrapping memory management in C++" 0)

    message(STATUS "Start freertos download")
    FetchContent_Declare( freertos_kernel
            GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Kernel.git
            GIT_TAG        V11.0.1
    )

    set( FREERTOS_HEAP "4" CACHE STRING "" FORCE)
    # Select the native compile PORT
    set( FREERTOS_PORT "MSVC_MINGW" CACHE STRING "" FORCE)
    # Select the cross-compile PORT
#        if (CMAKE_CROSSCOMPILING)
#            set(FREERTOS_PORT "GCC_ARM_CA9" CACHE STRING "" FORCE)
#        endif()
    include_directories(${CMAKE_BINARY_DIR}/_deps/freertos_kernel-src/include)
    include_directories(${CMAKE_BINARY_DIR}/_deps/freertos_kernel-src/portable/MSVC-MingW)

    FetchContent_MakeAvailable(freertos_kernel)

    set(PLATFORM_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/freertos ${CMAKE_CURRENT_SOURCE_DIR}/src/freertos/config)
    include_directories(${PLATFORM_INCLUDE})
    set(PLATFORM_LIB freertos_kernel)

    file(GLOB_RECURSE OSAL_INCLUDES CONFIGURE_DEPENDS "inc/*.hpp" "src/freertos/osal_sys/*.hpp" "src/freertos/config/*.h")
    file(GLOB_RECURSE OSAL_SOURCES CONFIGURE_DEPENDS "src/error.cpp" "src/log.cpp" "src/generics.cpp" "src/freertos/*.cpp")

    set(LOG_NEW_LINE \\r\\n)
elseif(ENABLE_FREERTOS)
    message(STATUS "OSAL for FreeRTOS")

#    include(FetchContent)
#    message(STATUS "Start download FreeRTOS Kernel")
#    FetchContent_Declare( freertos_kernel
#            GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Kernel.git
#            GIT_TAG        V11.0.1
#    )
#
#    add_library(freertos_config INTERFACE)
#
#    target_include_directories(freertos_config SYSTEM
#            INTERFACE
#            src/freertos/config
#    )
#
#    target_compile_definitions(freertos_config
#            INTERFACE
#            projCOVERAGE_TEST=0
#    )
#
#
#    set( FREERTOS_HEAP "4" CACHE STRING "" FORCE)
#    # Select the native compile PORT
#    set( FREERTOS_PORT "GCC_RP2040" CACHE STRING "" FORCE)
#    # Select the cross-compile PORT
#    #        if (CMAKE_CROSSCOMPILING)
#    #            set(FREERTOS_PORT "GCC_ARM_CA9" CACHE STRING "" FORCE)
#    #        endif()
#    include_directories(${CMAKE_BINARY_DIR}/_deps/freertos_kernel-src/include)
#    include_directories(${CMAKE_BINARY_DIR}/_deps/freertos_kernel-src/portable/ThirdParty/GCC/RP2040/include)
#
#    set(OS_MEM_LAYER 1)
#    set(PLATFORM_LIB "freertos_kernel")
#    FetchContent_MakeAvailable(freertos_kernel)
#
#    set(PLATFORM_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/freertos ${CMAKE_CURRENT_SOURCE_DIR}/src/freertos/config)
#    include_directories(${PLATFORM_INCLUDE})
    file(GLOB_RECURSE OSAL_INCLUDES CONFIGURE_DEPENDS "inc/*.hpp" "src/freertos/osal_sys/*.hpp" "src/freertos/config/*.h")
    file(GLOB_RECURSE OSAL_SOURCES CONFIGURE_DEPENDS "src/error.cpp" "src/log.cpp" "src/generics.cpp" "src/freertos/*.cpp")
else ()
    message(FATAL_ERROR "No one platform selected" )
endif()

message(STATUS "PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")
message(STATUS "OSAL_INCLUDES=${OSAL_INCLUDES}")
message(STATUS "OSAL_SOURCES=${OSAL_SOURCES}")


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/osal-config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config/osal-config.h)

#add_compile_options(-fno-rtti -fno-exceptions)
add_library(${PROJECT_NAME} STATIC ${OSAL_SOURCES} ${OSAL_INCLUDES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PLATFORM_INCLUDES})

if( supported )
    message(STATUS "IPO / LTO enabled for ${PROJECT_NAME}")
    set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS "IPO / LTO not supported for ${PROJECT_NAME}: <${error}>")
endif()

if (ENABLE_TEST)
    message(STATUS "build test")

    add_executable(osal_test ${OSAL_TEST})

    target_link_libraries(osal_test
                            osal
                            ${PLATFORM_LIB}
                            GTest::gtest
                            GTest::gtest_main
                            )

    if( supported )
        message(STATUS "IPO / LTO enabled for osal_test")
        set_property(TARGET osal_test PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(STATUS "IPO / LTO not supported for osal_test: <${error}>")
    endif()

endif()


# Doxygen configuration
cmake_policy(SET CMP0057 NEW)
find_package(Doxygen)
if (CMAKE_PROJECT_NAME STREQUAL OSAL AND DOXYGEN_FOUND)
  set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
  set(DOXYGEN_TYPEDEF_HIDES_STRUCT YES)
  set(DOXYGEN_EXTRACT_STATIC YES)
  set(DOXYGEN_STRIP_CODE_COMMENTS NO)
  set(DOXYGEN_MACRO_EXPANSION YES)
  set(DOXYGEN_EXPAND_ONLY_PREDEF YES)
  set(DOXYGEN_PREDEFINED OSAL_EXPORT)
  set(DOXYGEN_EXPAND_AS_DEFINED OSAL_EXPORT)
  set(DOXYGEN_COLLABORATION_GRAPH NO)
  set(DOXYGEN_INCLUDE_GRAPH NO)
  set(DOXYGEN_INCLUDED_BY_GRAPH NO)
  set(DOXYGEN_RECURSIVE NO)
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
  doxygen_add_docs(docs
    README.md
    include
    src
    )
endif()


