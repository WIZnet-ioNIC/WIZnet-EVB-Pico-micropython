/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2026 Diego Eckhard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "hardware/flash.h"
#include "hardware/sync.h"
#include "pico/unique_id.h"

// The W55RP20 has an internal Puya P25Q16 flash with a 128-bit unique ID.
// pico_get_unique_board_id() only uses the first 64 bits, and on these parts
// they are a lot code shared by many chips (eg 554d383836380019, "UM8868..").
// That makes machine.unique_id(), the default MAC address and the USB serial
// number the same on many boards.  Instead, read the full 128-bit ID and hash
// it down to the 64 bits expected by pico_get_unique_board_id().
//
// The board's mpconfigboard.cmake wraps pico_get_unique_board_id() so that
// all callers use this ID.

#define FLASH_RUID_CMD (0x4b)
#define FLASH_RUID_DUMMY_BYTES (4)
#define FLASH_RUID_BYTES (16)
#define FLASH_RUID_TOTAL_BYTES (1 + FLASH_RUID_DUMMY_BYTES + FLASH_RUID_BYTES)

static pico_unique_board_id_t board_unique_id;

// Run once at boot, like pico_unique_id does, while only core0 is running.
static void __attribute__((constructor)) board_unique_id_init(void) {
    uint8_t txbuf[FLASH_RUID_TOTAL_BYTES] = {FLASH_RUID_CMD};
    uint8_t rxbuf[FLASH_RUID_TOTAL_BYTES];
    uint32_t state = save_and_disable_interrupts();
    flash_do_cmd(txbuf, rxbuf, FLASH_RUID_TOTAL_BYTES);
    restore_interrupts(state);

    // 64-bit FNV-1a hash of the full flash unique ID.
    uint64_t h = 0xcbf29ce484222325ULL;
    for (size_t i = 0; i < FLASH_RUID_BYTES; ++i) {
        h = (h ^ rxbuf[1 + FLASH_RUID_DUMMY_BYTES + i]) * 0x100000001b3ULL;
    }
    for (size_t i = 0; i < PICO_UNIQUE_BOARD_ID_SIZE_BYTES; ++i) {
        board_unique_id.id[i] = h >> (8 * i);
    }
}

void __wrap_pico_get_unique_board_id(pico_unique_board_id_t *id_out) {
    *id_out = board_unique_id;
}
