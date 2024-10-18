#ifndef MICROPY_INCLUDED_MPCONFIGPORT_H
#define MICROPY_INCLUDED_MPCONFIGPORT_H

#include <rtthread.h>
#include <hal_data.h>

// options to control how MicroPython is built
// Config level
#define MICROPY_CONFIG_ROM_LEVEL (MICROPY_CONFIG_ROM_LEVEL_FULL_FEATURES)

// Memory allocation policies
#ifdef BSP_USING_SDRAM
#define MICROPY_GC_STACK_ENTRY_TYPE         uint32_t
#else
#define MICROPY_GC_STACK_ENTRY_TYPE         uint16_t
#endif
#define MICROPY_ALLOC_PATH_MAX              (128)

// Optimisations

// MicroPython emitters
#define MICROPY_PERSISTENT_CODE_LOAD        (1)
#define MICROPY_EMIT_THUMB                  (1)
#define MICROPY_EMIT_INLINE_THUMB           (1)

// Python internal features
#define MICROPY_TRACKED_ALLOC               (MICROPY_SSL_MBEDTLS)
#define MICROPY_READER_VFS                  (1)
#define MICROPY_ENABLE_GC                   (1)
#define MICROPY_ENABLE_EMERGENCY_EXCEPTION_BUF  (1)
#define MICROPY_LONGINT_IMPL                (MICROPY_LONGINT_IMPL_MPZ)
#define MICROPY_FLOAT_IMPL                  (MICROPY_FLOAT_IMPL_DOUBLE)
#define MICROPY_SCHEDULER_DEPTH             (8)
#define MICROPY_SCHEDULER_STATIC_NODES      (1)
#define MICROPY_ENABLE_SCHEDULER            (1)
#define MICROPY_PY_SYS						(1)
#define MICROPY_PY_THREAD                   (1)
#define MICROPY_PY_THREAD_GIL				(1)
#define MICROPY_MODULE_FROZEN_MPY			(0)
#define MICROPY_VFS                         (1)
#define MICROPY_VFS_POSIX                   (0)
#define MICROPY_QSTR_EXTRA_POOL             mp_qstr_frozen_const_pool

// Control over Python builtins
#define MICROPY_PY_SYS_PLATFORM             "renesas"

/* rbbbb*/
#define MICROPY_HW_ENABLE_UART_REPLC		(0)
#define MICROPY_HW_USB_CDC					(1)
/* rbbbb*/

// Extended modules
#define MICROPY_EPOCH_IS_1970               (1)
#define MICROPY_PY_TIME_GMTIME_LOCALTIME_MKTIME (1)
#define MICROPY_PY_TIME_TIME_TIME_NS        (1)
#define MICROPY_PY_TIME_INCLUDEFILE         "port/rtthread_port/modtime.c"
extern uint32_t rosc_random_u32(void);
#define MICROPY_PY_RANDOM_SEED_INIT_FUNC    (rosc_random_u32())
#define MICROPY_PY_OS_DUPTERM               (3)
#define MICROPY_PY_OS_DUPTERM_NOTIFY        (1)
#define MICROPY_PY_OS_SYNC                  (1)
#define MICROPY_PY_OS_UNAME                 (1)
#define MICROPY_PY_OS_URANDOM               (0)
#define MICROPY_PY_MACHINE                  (1)
#define MICROPY_PY_MACHINE_UART             (1)
#define MICROPY_PY_MACHINE_PIN				(1)
#define MICROPY_PY_MACHINE_I2C				(1)
#define MICROPY_PY_MACHINE_ADC				(1)
#define MICROPY_PY_MACHINE_PWM				(1)

#define MICROPY_HW_BOARD_NAME				"Vision Board"
#define MICROPY_HW_MCU_NAME					"renesas-ra8d1"

#define MP_STATE_PORT MP_STATE_VM

#if MICROPY_PY_THREAD
#define MICROPY_EVENT_POLL_HOOK \
    do { \
        extern void mp_handle_pending(bool); \
        mp_handle_pending(true); \
        MP_THREAD_GIL_EXIT(); \
        rt_thread_yield(); \
        MP_THREAD_GIL_ENTER(); \
    } while (0);

#define MICROPY_THREAD_YIELD() rt_thread_yield()
#else
#define MICROPY_EVENT_POLL_HOOK \
    do { \
        extern void mp_handle_pending(bool); \
        mp_handle_pending(true); \
        __WFI(); \
    } while (0);

#define MICROPY_THREAD_YIELD()
#endif

#define MICROPY_MAKE_POINTER_CALLABLE(p) ((void *)((mp_uint_t)(p) | 1))

#define MP_HAL_CLEANINVALIDATE_DCACHE(addr, size) \
    (SCB_CleanInvalidateDCache_by_Addr((uint32_t *)((uint32_t)addr & ~0x1f), \
    ((uint32_t)((uint8_t *)addr + size + 0x1f) & ~0x1f) - ((uint32_t)addr & ~0x1f)))

#define MP_HAL_CLEAN_DCACHE(addr, size) \
    (SCB_CleanDCache_by_Addr((uint32_t *)((uint32_t)addr & ~0x1f), \
    ((uint32_t)((uint8_t *)addr + size + 0x1f) & ~0x1f) - ((uint32_t)addr & ~0x1f)))

// type definitions for the specific machine
#define SEEK_SET    0
#define SEEK_CUR    1

#define MP_SSIZE_MAX (0x7fffffff)
typedef int mp_int_t; // must be pointer size
typedef unsigned mp_uint_t; // must be pointer size
typedef long mp_off_t;

// We need to provide a declaration/definition of alloca()
#include <alloca.h>

#endif //  MICROPY_INCLUDED_MPCONFIGPORT_H
