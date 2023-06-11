#********************************************************************
#        _       _         _
#  _ __ | |_  _ | |  __ _ | |__   ___
# | '__|| __|(_)| | / _` || '_ \ / __|
# | |   | |_  _ | || (_| || |_) |\__ \
# |_|    \__|(_)|_| \__,_||_.__/ |___/
#
# www.rt-labs.com
# Copyright 2017 rt-labs AB, Sweden.
#
# This software is licensed under the terms of the BSD 3-clause
# license. See the file LICENSE distributed with this software for
# full license information.
#*******************************************************************/

find_package(Threads)

option (USE_SCHED_FIFO
  "Use SCHED_FIFO policy. May require extra privileges to run"
  OFF)

target_sources(OSAL PRIVATE
  src/linux/OSAL.c
  src/linux/osal_log.c
  )

target_compile_options(OSAL
  PRIVATE
  -Wall
  -Wextra
  -Werror
  -Wno-unused-parameter
  $<$<BOOL:${USE_SCHED_FIFO}>:-DUSE_SCHED_FIFO>
  INTERFACE
  $<$<CONFIG:Coverage>:--coverage>
  )

target_include_directories(OSAL PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/linux>
  )

target_link_libraries(OSAL PUBLIC
  Threads::Threads
  rt
  INTERFACE
  $<$<CONFIG:Coverage>:--coverage>
  )

install(FILES
  src/linux/sys/osal_cc.h
  src/linux/sys/osal_sys.h
  DESTINATION include/sys
  )

if (BUILD_TESTING)
  set(GOOGLE_TEST_INDIVIDUAL TRUE)
endif()
