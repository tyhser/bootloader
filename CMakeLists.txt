cmake_minimum_required(VERSION 3.0)
PROJECT(bootloader LANGUAGES C ASM)


set(BOOTLOADER_SOURCE_DIR ${PROJECT_SOURCE_DIR})

aux_source_directory(. SRC_LIST)
aux_source_directory(${BOOTLOADER_SOURCE_DIR}/arch/chip1/driver/src SRC_DRV_LIST)
set(SRC_LIST "${SRC_LIST};${SRC_DRV_LIST};${BOOTLOADER_SOURCE_DIR}/arch/chip1/start.S")

include_directories(.
		${BOOTLOADER_SOURCE_DIR}/arch/chip1/driver/inc
		${BOOTLOADER_SOURCE_DIR}/arch/include
		${BOOTLOADER_SOURCE_DIR}/arch/chip1/include
		)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g -T${BOOTLOADER_SOURCE_DIR}/arch/chip1/link.ld")

MESSAGE("file list:
	------------------------------
	${SRC_LIST}"
	)
MESSAGE("	------------------------------")
add_executable(bootloader ${SRC_LIST})
