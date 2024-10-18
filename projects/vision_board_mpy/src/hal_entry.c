/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2023-12-17     Rbb666        first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include "ra/board/ra8d1_ek/board_sdram.h"
#ifdef RT_USING_DFS
#include <unistd.h>
#include <fcntl.h>
#endif
#ifdef BSP_USING_OPENMV
#include <py/compile.h>
#include <py/runtime.h>
#include <py/repl.h>
#include <py/gc.h>
#include <py/mperrno.h>
#include <py/stackctrl.h>
#include <py/frozenmod.h>
#include <py/mphal.h>
#include "shared/runtime/pyexec.h"
#include "shared/readline/readline.h"
#include "mp_utils.h"
#include <framebuffer.h>
#include <fb_alloc.h>
#include <usbdbg.h>
#include <sensor.h>
#include <tinyusb_debug.h>
#include <tusb.h>
#endif /* BSP_USING_OPENMV */
#ifdef RT_USING_FAL
#include "fal.h"
#endif /* RT_USING_FAL */

#define DRV_DEBUG
#define LOG_TAG "main"
#include <drv_log.h>

/* MicroPython runs as a task under RT-Thread */
#define MP_TASK_STACK_SIZE (64 * 1024)

#ifdef BSP_USING_OPENMV
// static char gc_heap[OMV_HEAP_SIZE] BSP_ALIGN_VARIABLE(4) BSP_PLACE_IN_SECTION(".sdram");
extern uint8_t _gc_heap_start, _gc_heap_end;
static void *stack_top = RT_NULL;

extern int mount_init(void);
extern void fmath_init(void);
extern int tusb_board_init(void);

void *__signgam_addr(void)
{
    return NULL;
}

static void omv_entry(void *parameter)
{
    (void)parameter;
    int stack_dummy;
    stack_top = (void *)&stack_dummy;

    bool first_soft_reset = true;

    tusb_board_init();
#ifdef RT_USING_FAL
    fal_init();
#endif
#ifdef BSP_USING_FS
    /* wait sdcard mount */
    extern struct rt_semaphore sem_mnt_lock;
    rt_sem_take(&sem_mnt_lock, 400);

//    struct dfs_fdtable *fd_table_bak = NULL;
#endif
//    fmath_init();
soft_reset:
    // #ifdef BSP_USING_FS
    //     mp_sys_resource_bak(&fd_table_bak);
    // #endif
    /* Stack limit should be less than real stack size, so we have a */
    /* chance to recover from limit hit. (Limit is measured in bytes) */
    mp_stack_set_top(stack_top);
    mp_stack_set_limit(MP_TASK_STACK_SIZE - 1024);
    /* GC init */
    // gc_init(&gc_heap[0], &gc_heap[MP_ARRAY_SIZE(gc_heap)]);
    gc_init(&_gc_heap_start, &_gc_heap_end);
#if MICROPY_PY_THREAD
    mp_thread_init(rt_thread_self()->stack_addr, MP_TASK_STACK_SIZE / sizeof(uintptr_t));
#endif
    /* MicroPython initialization */
    mp_init();

    imlib_init_all();
    readline_init0();
    fb_alloc_init0();
    framebuffer_init0();
    sensor_init0();
#ifdef IMLIB_ENABLE_IMAGE_FILE_IO
    file_buffer_init0();
#endif
    usbdbg_init();

#if MICROPY_PY_SENSOR
    if (sensor_init() != 0 && first_soft_reset)
    {
        LOG_E("sensor init failed!");
    }
#endif

    extern int usb_cdc_init(void);
    usb_cdc_init();

    // Run boot.py script.
    bool interrupted = mp_exec_bootscript("boot.py", true, false);

    // Run main.py script on first soft-reset.
    if (first_soft_reset && !interrupted)
    {
        mp_exec_bootscript("main.py", true, false);
        goto soft_reset_exit;
    }

    /* If there's no script ready, just re-exec REPL */
    while (!usbdbg_script_ready())
    {
        nlr_buf_t nlr;
        if (nlr_push(&nlr) == 0)
        {
            /* enable IDE interrupt */
            usbdbg_set_irq_enabled(true);
            /* run REPL */
            if (pyexec_mode_kind == PYEXEC_MODE_RAW_REPL)
            {
                if (pyexec_raw_repl() != 0)
                {
                    break;
                }
            }
            else
            {
                if (pyexec_friendly_repl() != 0)
                {
                    break;
                }
            }
            nlr_pop();
        }
    }

    LOG_I("Exit from repy!");

    if (usbdbg_script_ready())
    {
        nlr_buf_t nlr;
        if (nlr_push(&nlr) == 0)
        {
            /* Enable IDE interrupt */
            usbdbg_set_irq_enabled(true);
            /* Execute the script */
            pyexec_str(usbdbg_get_script(), true);
            /* Disable IDE interrupts */
            usbdbg_set_irq_enabled(false);
            nlr_pop();
        }
        else
        {
            mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
        }

        if (usbdbg_is_busy() && nlr_push(&nlr) == 0)
        {
            /* Enable IDE interrupt */
            usbdbg_set_irq_enabled(true);
            /* Wait for the current command to finish */
            usbdbg_wait_for_command(1000);
            /* Disable IDE interrupts */
            usbdbg_set_irq_enabled(false);
            nlr_pop();
        }
    }

soft_reset_exit:
    /* soft reset */
    mp_printf(&mp_plat_print, "MPY: soft reboot\n");

    gc_sweep_all();
    mp_deinit();
#if MICROPY_PY_THREAD
    mp_thread_deinit();
#endif
#ifdef RT_USING_DFS
//    mp_sys_resource_gc(fd_table_bak);
#endif
// #ifdef OPENMV_USING_KEY
//     rt_uint32_t pin = rt_pin_get(USER_KEY_PIN_NAME);
//     rt_pin_detach_irq(pin);
// #endif
    first_soft_reset = false;
    goto soft_reset;
}

static void omv_init_func(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("omv", omv_entry, RT_NULL,
                           MP_TASK_STACK_SIZE / sizeof(uint32_t), 22, 20);
    RT_ASSERT(tid != RT_NULL);

    rt_thread_startup(tid);
}
#endif /* BSP_USING_OPENMV */

void hal_entry(void)
{
    LOG_I("===================================");
    LOG_I("This is OpenMV4.5.4 demo");
    LOG_I("===================================");

#ifdef BSP_USING_OPENMV
    omv_init_func();
#endif
}
