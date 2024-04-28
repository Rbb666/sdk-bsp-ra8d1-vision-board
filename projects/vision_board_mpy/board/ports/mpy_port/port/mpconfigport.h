#include <stdint.h>
#include <rtthread.h>

// options to control how MicroPython is built
// Config level
#define MICROPY_CONFIG_ROM_LEVEL (MICROPY_CONFIG_ROM_LEVEL_FULL_FEATURES)

// Memory allocation policies
#ifdef BSP_USING_SDRAM
#define MICROPY_GC_STACK_ENTRY_TYPE         uint32_t
#else
#define MICROPY_GC_STACK_ENTRY_TYPE         uint16_t
#endif
#define MICROPY_ALLOC_PATH_MAX              (256)

// Optimisations

// Python internal features
#define MICROPY_TRACKED_ALLOC               (MICROPY_SSL_MBEDTLS)
#define MICROPY_READER_VFS                  (0)
#define MICROPY_ENABLE_GC                   (1)
#define MICROPY_HELPER_REPL                 (1)
#define MICROPY_ENABLE_EMERGENCY_EXCEPTION_BUF  (1)
#define MICROPY_LONGINT_IMPL                (MICROPY_LONGINT_IMPL_MPZ)
#define MICROPY_FLOAT_IMPL                  (MICROPY_FLOAT_IMPL_DOUBLE)
#define MICROPY_SCHEDULER_DEPTH             (8)
#define MICROPY_SCHEDULER_STATIC_NODES      (1)
#define MICROPY_ENABLE_SCHEDULER            (1)
#define MICROPY_PY_THREAD                   (1)
#define MICROPY_VFS                         (0)
#define MICROPY_QSTR_EXTRA_POOL             mp_qstr_frozen_const_pool

// Control over Python builtins
#define MICROPY_PY_SYS_PLATFORM             "renesas"

// MicroPython emitters
#define MICROPY_PERSISTENT_CODE_LOAD        (1)
#define MICROPY_EMIT_THUMB                  (1)
#define MICROPY_EMIT_INLINE_THUMB           (1)
#define MICROPY_USE_INTERNAL_ERRNO          (1)
#define MICROPY_KBD_EXCEPTION               (0)
#define MICROPY_PY_IO                       (1)

/* rbbbb*/
#define MICROPY_PY_MATH                     (1)
#define MICROPY_PY_MATH_SPECIAL_FUNCTIONS   (1)
#define MICROPY_PY_BUILTINS_FLOAT           (1)
/* rbbbb*/

// Extended modules
#define MICROPY_PY_ONEWIRE                  (0)
#define MICROPY_PY_MACHINE                  (0)
#define MICROPY_PY_MACHINE_UART             (0)


#define MICROPY_HW_BOARD_NAME "Vision Board"
#define MICROPY_HW_MCU_NAME "renesas-ra8d1"

// We need to provide a declaration/definition of alloca()
#include <alloca.h>

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
