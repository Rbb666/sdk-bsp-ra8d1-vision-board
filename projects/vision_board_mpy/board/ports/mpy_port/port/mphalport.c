/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Armink (armink.ztl@gmail.com)
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

#include <stdio.h>
#include <string.h>

#include <rtthread.h>
#include "py/mphal.h"
#include "py/stream.h"
#include "py/mpconfig.h"
#include "py/runtime.h"
#include "py/ringbuf.h"
#include "extmod/misc.h"
#include "shared/runtime/interrupt_char.h"

#ifdef PKG_USING_TINYUSB
    #include "tusb.h"
#endif

const char rtthread_help_text[] =
    "Welcome to MicroPython on RT-Thread!\n"
    "\n"
    "Control commands:\n"
    "  CTRL-A        -- on a blank line, enter raw REPL mode\n"
    "  CTRL-B        -- on a blank line, enter normal REPL mode\n"
    "  CTRL-C        -- interrupt a running program\n"
    "  CTRL-D        -- on a blank line, do a soft reset of the board\n"
    "  CTRL-E        -- on a blank line, enter paste mode\n"
    "\n"
    "For further help on a specific object, type help(obj)\n"
    ;

#if MICROPY_HW_USB_CDC

extern ringbuf_t debug_ringbuf;
extern bool tinyusb_debug_enabled(void);

#ifndef MICROPY_HW_STDIN_BUFFER_LEN
    #define MICROPY_HW_STDIN_BUFFER_LEN 512
#endif

static uint8_t stdin_ringbuf_array[MICROPY_HW_STDIN_BUFFER_LEN];
ringbuf_t stdin_ringbuf = {stdin_ringbuf_array, sizeof(stdin_ringbuf_array), 0, 0};
#endif

#if !MICROPY_KBD_EXCEPTION
void mp_hal_set_interrupt_char(int c);
#endif

#if MICROPY_HW_USB_CDC
uint8_t cdc_itf_pending; // keep track of cdc interfaces which need attention to poll

void poll_cdc_interfaces(void)
{
    // any CDC interfaces left to poll?
    if (cdc_itf_pending && ringbuf_free(&stdin_ringbuf))
    {
        for (uint8_t itf = 0; itf < 8; ++itf)
        {
            if (cdc_itf_pending & (1 << itf))
            {
                tud_cdc_rx_cb(itf);
                if (!cdc_itf_pending)
                {
                    break;
                }
            }
        }
    }
}

void tud_cdc_rx_cb(uint8_t itf)
{
	if (tinyusb_debug_enabled()) {
        return;
	}

    // consume pending USB data immediately to free usb buffer and keep the endpoint from stalling.
    // in case the ringbuffer is full, mark the CDC interface that need attention later on for polling
    cdc_itf_pending &= ~(1 << itf);
    for (uint32_t bytes_avail = tud_cdc_n_available(itf); bytes_avail > 0; --bytes_avail)
    {
        if (ringbuf_free(&stdin_ringbuf))
        {
            int data_char = tud_cdc_read_char();
            if (data_char == mp_interrupt_char)
            {
                mp_sched_keyboard_interrupt();
            }
            else
            {
                ringbuf_put(&stdin_ringbuf, data_char);
            }
        }
        else
        {
            cdc_itf_pending |= (1 << itf);
            return;
        }
    }
}
#endif

int mp_hal_stdin_rx_chr(void)
{
    for (;;)
    {
#if MICROPY_HW_USB_CDC
        poll_cdc_interfaces();
#endif

        int c = ringbuf_get(&stdin_ringbuf);
        if (c != -1)
        {
            return c;
        }
#if MICROPY_PY_OS_DUPTERM
        int dupterm_c = mp_os_dupterm_rx_chr();
        if (dupterm_c >= 0)
        {
            return dupterm_c;
        }
#endif
        MICROPY_EVENT_POLL_HOOK
    }
}

// Send string of given length
uintptr_t mp_hal_stdio_poll(uintptr_t poll_flags)
{
    uintptr_t ret = 0;
#if MICROPY_HW_USB_CDC
    poll_cdc_interfaces();
#endif
#if MICROPY_HW_ENABLE_UART_REPL || MICROPY_HW_USB_CDC
    if ((poll_flags & MP_STREAM_POLL_RD) && ringbuf_peek(&stdin_ringbuf) != -1)
    {
        ret |= MP_STREAM_POLL_RD;
    }
    if (poll_flags & MP_STREAM_POLL_WR)
    {
#if MICROPY_HW_ENABLE_UART_REPL
        ret |= MP_STREAM_POLL_WR;
#else
        if (tud_cdc_connected() && tud_cdc_write_available() > 0)
        {
            ret |= MP_STREAM_POLL_WR;
        }
#endif
    }
#endif
#if MICROPY_PY_OS_DUPTERM
    ret |= mp_os_dupterm_poll(poll_flags);
#endif
    return ret;
}

// Send string of given length
mp_uint_t mp_hal_stdout_tx_strn(const char *str, mp_uint_t len)
{
    mp_uint_t ret = len;
    bool did_write = false;
#if MICROPY_HW_ENABLE_UART_REPL
    mp_putsn(str, len);
#endif

#if MICROPY_HW_USB_CDC
	if (tinyusb_debug_enabled()) {
        if (tud_cdc_connected()) {
            for (int i = 0; i < len; i++) {
                rt_base_t level;
				level = rt_hw_interrupt_disable();
				ringbuf_put((ringbuf_t *) &debug_ringbuf, str[i]);
				rt_hw_interrupt_enable(level);
            }
        }
        return ret;
    }

    if (tud_cdc_connected())
    {
        size_t i = 0;
        while (i < len)
        {
            uint32_t n = len - i;
            if (n > CFG_TUD_CDC_EP_BUFSIZE)
            {
                n = CFG_TUD_CDC_EP_BUFSIZE;
            }
            int timeout = 0;
            // Wait with a max of USC_CDC_TIMEOUT ms
            while (n > tud_cdc_write_available() && timeout++ < MICROPY_HW_USB_CDC_TX_TIMEOUT)
            {
                MICROPY_EVENT_POLL_HOOK
            }
            if (timeout >= MICROPY_HW_USB_CDC_TX_TIMEOUT)
            {
                break;
            }
            uint32_t n2 = tud_cdc_write(str + i, n);
            tud_cdc_write_flush();
            i += n2;
        }
        ret = MIN(i, ret);
        did_write = true;
    }
#endif

#if MICROPY_PY_OS_DUPTERM
    mp_os_dupterm_tx_strn(str, len);
#endif

    return did_write ? ret : 0;
}

uint32_t rng_randint(uint32_t min, uint32_t max) {
	return 0;
}

mp_uint_t mp_hal_ticks_us(void)
{
    return rt_tick_get() * 1000000UL / RT_TICK_PER_SECOND;
}

mp_uint_t mp_hal_ticks_ms(void)
{
    return rt_tick_get_millisecond();
}

mp_uint_t mp_hal_ticks_cpu(void)
{
    return rt_tick_get();
}

void mp_hal_delay_us(mp_uint_t us)
{
    rt_tick_t t0 = rt_tick_get(), t1, dt;
    uint64_t dtick = us * RT_TICK_PER_SECOND / 1000000L;
    while (1)
    {
        t1 = rt_tick_get();
        dt = t1 - t0;
        if (dt >= dtick)
        {
            break;
        }
        mp_handle_pending(true);
    }
}

void mp_hal_delay_ms(mp_uint_t ms)
{
    rt_tick_t t0 = rt_tick_get(), t1, dt;
    uint64_t dtick = ms * RT_TICK_PER_SECOND / 1000L;
    while (1)
    {
        t1 = rt_tick_get();
        dt = t1 - t0;
        if (dt >= dtick)
        {
            break;
        }
        MICROPY_EVENT_POLL_HOOK;
        rt_thread_delay(1);
    }
}

uint64_t mp_hal_time_ns(void)
{
	uint64_t ns = 0;
	return ns;
}
