/*
 * This file is part of the OpenMV project.
 *
 * Copyright (c) 2022 Ibrahim Abdelkader <iabdalkader@openmv.io>
 * Copyright (c) 2022 Kwabena W. Agyeman <kwagyeman@openmv.io>
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * Tinyusb CDC debugger helper code.
 */

#include "omv_boardconfig.h"
#if (OMV_TUSBDBG_ENABLE == 1)
#include "py/runtime.h"
#include "py/stream.h"
#include "py/mphal.h"
#include "py/ringbuf.h"
#include "omv_common.h"

#include "tusb.h"
#include "usbdbg.h"
#include "tinyusb_debug.h"

#define DEBUG_MAX_PACKET        (OMV_TUSBDBG_PACKET)
#define DEBUG_BAUDRATE_SLOW     (921600)
#define DEBUG_BAUDRATE_FAST     (12000000)
#define DEBUG_EP_SIZE           (TUD_OPT_HIGH_SPEED ? 512 : 64)

extern void __fatal_error();

typedef struct __attribute__((packed)) {
    uint8_t cmd;
    uint8_t request;
    uint32_t xfer_length;
}
usbdbg_cmd_t;

static uint8_t debug_ringbuf_array[512];
static volatile bool tinyusb_debug_mode = false;
ringbuf_t debug_ringbuf = { debug_ringbuf_array, sizeof(debug_ringbuf_array) };

uint32_t usb_cdc_buf_len() {
    return ringbuf_avail((ringbuf_t *) &debug_ringbuf);
}

uint32_t usb_cdc_get_buf(uint8_t *buf, uint32_t len) {
    for (int i = 0; i < len; i++) {
        buf[i] = ringbuf_get((ringbuf_t *) &debug_ringbuf);
    }
    return len;
}

__attribute__((__used__))
void tud_cdc_line_coding_cb(uint8_t itf, cdc_line_coding_t const *coding) {
    debug_ringbuf.iget = 0;
    debug_ringbuf.iput = 0;

    if (0) {
        #if defined(MICROPY_BOARD_ENTER_BOOTLOADER)
    } else if (coding->bit_rate == 1200) {
        MICROPY_BOARD_ENTER_BOOTLOADER(0, 0);
        #endif
    } else if (coding->bit_rate == DEBUG_BAUDRATE_SLOW
               || coding->bit_rate == DEBUG_BAUDRATE_FAST) {
        tinyusb_debug_mode = true;
    } else {
        tinyusb_debug_mode = false;
    }
}

bool tinyusb_debug_enabled(void) {
    return tinyusb_debug_mode;
}

static void tinyusb_debug_task(void) {
	tud_task();

    if (!tinyusb_debug_enabled() || !tud_cdc_connected() || tud_cdc_available() < 6) {
        return;
    }

    uint8_t dbg_buf[DEBUG_MAX_PACKET];
    uint32_t count = tud_cdc_read(dbg_buf, 6);

    if (count < 6 || dbg_buf[0] != 0x30) {
        // Maybe we should try to recover from this state
        // but for now, call __fatal_error which doesn't
        // return.
        __fatal_error();
        return;
    }

    usbdbg_cmd_t *cmd = (usbdbg_cmd_t *) dbg_buf;
    uint8_t request = cmd->request;
    uint32_t xfer_length = cmd->xfer_length;
    usbdbg_control(NULL, request, xfer_length);

    while (xfer_length) {
        // && tud_cdc_connected())
        if (tud_task_event_ready()) {
			tud_task();
        }
        if (request & 0x80) {
            // Device-to-host data phase
            int size = OMV_MIN(xfer_length, tud_cdc_write_available());
            if (size) {
                xfer_length -= size;
                usbdbg_data_in(size, tud_cdc_write);
            }
            if (size > 0 && size < DEBUG_EP_SIZE) {
                tud_cdc_write_flush();
            }
        } else {
            // Host-to-device data phase
            int size = OMV_MIN(xfer_length, tud_cdc_available());
            if (size) {
                xfer_length -= size;
                usbdbg_data_out(size, tud_cdc_read);
            }
        }
    }
}

static void tusb_thread_entry(void *parameter)
{
    (void) parameter;
    while (1)
    {
        tinyusb_debug_task();
    }
}

/* if there is not enable heap, we should use static thread and stack. */
static rt_uint8_t tusb_stack[PKG_TINYUSB_STACK_SIZE];
static struct rt_thread tusb_thread;

static int init_tinyusb(void)
{
    rt_thread_t tid;
    rt_err_t result;

    tusb_init();
    tid = &tusb_thread;
    result = rt_thread_init(tid, "usb", tusb_thread_entry, RT_NULL,
                            tusb_stack, sizeof(tusb_stack), PKG_TINYUSB_THREAD_PRIORITY, 10);
    if (result != RT_EOK)
    {
        printf("Fail to create tinyusb thread\n");
        return -1;
    }
    rt_thread_startup(tid);
    return 0;
}

int usb_cdc_init(void)
{
    static bool initialized = false;
    if (!initialized)
    {
        initialized = true;
        init_tinyusb();
    }
    return 0;
}

#endif //OMV_TINYUSB_DEBUG
