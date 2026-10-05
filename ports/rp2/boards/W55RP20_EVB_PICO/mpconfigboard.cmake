# cmake file for Wiznet W55RP20-EVB-Pico.
set(PICO_BOARD wiznet_w5100s_evb_pico)
set(MICROPY_PY_NETWORK_WIZNET6K W5500)
set(MICROPY_WIZNET_PIO 1)
set(MICROPY_PY_NETWORK 1)
set(MICROPY_PY_LWIP 1)
set(MICROPY_FROZEN_MANIFEST ${MICROPY_BOARD_DIR}/manifest.py)

# Derive the unique ID from the full 128-bit flash unique ID, see board_unique_id.c.
# The wrap is deferred because the firmware target doesn't exist yet at this point.
set(MICROPY_SOURCE_BOARD ${MICROPY_BOARD_DIR}/board_unique_id.c)
cmake_language(DEFER CALL target_link_options ${MICROPY_TARGET} PRIVATE "LINKER:--wrap=pico_get_unique_board_id")

if(NOT DEFINED MICROPY_HW_FLASH_STORAGE_BYTES)
    set(MICROPY_HW_FLASH_STORAGE_BYTES 1441792)  # 1408 * 1024
endif()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-but-set-variable -Wno-unused-variable -Wno-misleading-indentation -Wno-incompatible-pointer-types -Wno-error=unused-function -Wno-comment -Wno-unused-function")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb -mcpu=cortex-m0plus")