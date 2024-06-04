
# Change Log
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

## [1.1.1] - 2024-06-04

Here we would have the update steps for 1.0.0 for people to follow.

### Fixed

- fix: remove one inline in event::clear_from_isr
- fix: concurrence on log print

## [1.0.0] - 2024-02-28

## [1.1.0] - 2024-06-02

Here we would have the update steps for 1.0.0 for people to follow.

### Added
 - add: instance replace in string
 - add: error type
 - add: FreeRTOS raw config

### Changed

- change: add ENABLE_TEST

### Fixed

 - fix: thread naming issue
 - fix: add check in thread to verify if handler is null
 - fix: in pico arch removed strnlen -> strlen
 - fix: string::replace refactoring and some small issue
 - fix: add string final
 - fix: some issue on string and memory
 - fix: compiling issue on unix platform
 - fix: remove NULL -> nullptr
 - fix: remove a deprecated os_free() function
 - fix: removed inline in some method
 - fix: deprecated os_malloc and os_free
 - fix: uninitialised field
 - fix: change a NULL to nullptr
 - fix: [[return]] in void function
 - fix: header loop include in freertos
 - fix: issue on set a function with 2 param in generics

## [1.0.0] - 2024-02-28

### Changed
 - First release