add_subdirectory(driver)

set(SRC_LIST "${SRC_LIST} ${BOOTLOADER_SOURCE_DRI}/arch/chip1/start.S" PARENT_SCOPE)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -T./arch/chip1/link.ld" PARENT_SCOPE)
