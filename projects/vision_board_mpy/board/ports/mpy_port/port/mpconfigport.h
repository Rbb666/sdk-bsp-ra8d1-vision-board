#include <stdint.h>
#include <rtthread.h>

// options to control how MicroPython is built

// Use the minimal starting configuration (disables all optional features).
#define MICROPY_CONFIG_ROM_LEVEL (MICROPY_CONFIG_ROM_LEVEL_FULL_FEATURES)

// You can disable the built-in MicroPython compiler by setting the following
// config option to 0.  If you do this then you won't get a REPL prompt, but you
// will still be able to execute pre-compiled scripts, compiled with mpy-cross.

#if defined(PKG_MICROPYTHON_HEAP_SIZE)
    #define MICROPY_HEAP_SIZE PKG_MICROPYTHON_HEAP_SIZE
#else
    #define MICROPY_HEAP_SIZE (8 * 1024)
#endif

#define MICROPY_ENABLE_COMPILER     (1)

#define MICROPY_QSTR_EXTRA_POOL             mp_qstr_frozen_const_pool
#define MICROPY_ENABLE_GC                   (1)
#define MICROPY_HELPER_REPL                 (1)

#define MICROPY_MODULE_FROZEN_MPY           (1)
#define MICROPY_ENABLE_EXTERNAL_IMPORT      (1)
#define MICROPY_QSTR_BYTES_IN_HASH          (2)
#define MICROPY_USE_INTERNAL_ERRNO          (1)
#define MICROPY_KBD_EXCEPTION               (1)
#define MICROPY_PY_BUILTINS_STR_OP_MODULO   (1)
#define MICROPY_ENABLE_SCHEDULER            (1)
#define MICROPY_PY_THREAD                   (1)
#define MICROPY_STACK_CHECK                 (1)
#define MICROPY_PY_IO                       (1)
#define MICROPY_ALLOC_PATH_MAX              (256)

// Use the minimum headroom in the chunk allocator for parse nodes.
#define MICROPY_ALLOC_PARSE_CHUNK_INIT    (16)

// type definitions for the specific machine
#define SEEK_SET    0
#define SEEK_CUR    1

typedef intptr_t mp_int_t; // must be pointer size
typedef uintptr_t mp_uint_t; // must be pointer size
typedef long mp_off_t;

// We need to provide a declaration/definition of alloca()
#include <alloca.h>

#define MICROPY_HW_BOARD_NAME "minimal"
#define MICROPY_HW_MCU_NAME "unknown-cpu"

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
