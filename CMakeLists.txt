cmake_minimum_required(VERSION 3.0)
PROJECT(bootloader LANGUAGES C ASM)

aux_source_directory(. SRC_LIST)

add_subdirectory(arch)

set(BOOTLOADER_SOURCE_DRI ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(.
		${BOOTLOADER_SOURCE_DRI}/arch/chip1/driver/inc
		${BOOTLOADER_SOURCE_DRI}/arch/include
		${BOOTLOADER_SOURCE_DRI}/arch/chip1/include
		)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g -T${BOOTLOADER_SOURCE_DRI}/arch/chip1/link.ld")

MESSAGE("Root file: ${SRC_LIST}")
add_executable(bootloader ${SRC_LIST})
