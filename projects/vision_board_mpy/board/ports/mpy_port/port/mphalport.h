#include "shared/runtime/interrupt_char.h"
#include "irq.h"

#define MICROPY_PY_PENDSV_ENTER   uint32_t atomic_state = raise_irq_pri(IRQ_PRI_PENDSV)
#define MICROPY_PY_PENDSV_EXIT    restore_irq_pri(atomic_state)

#define MICROPY_HW_USB_CDC_TX_TIMEOUT   (500)

// This macro is used to implement PEP 475 to retry specified syscalls on EINTR
#define MP_HAL_RETRY_SYSCALL(ret, syscall, raise) { \
        for (;;) { \
            MP_THREAD_GIL_EXIT(); \
            ret = syscall; \
            MP_THREAD_GIL_ENTER(); \
            if (ret == -1) { \
                int err = errno; \
                if (err == EINTR) { \
                    mp_handle_pending(true); \
                    continue; \
                } \
                raise; \
            } \
            break; \
        } \
}
