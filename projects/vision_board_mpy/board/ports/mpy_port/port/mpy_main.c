#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "py/builtin.h"
#include "py/compile.h"
#include "py/runtime.h"
#include "py/stackctrl.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "shared/runtime/pyexec.h"
#include "shared/readline/readline.h"

#include "mpgetcharport.h"
#include "mpputsnport.h"

#include <rtthread.h>

#if MICROPY_ENABLE_COMPILER
void do_str(const char *src, mp_parse_input_kind_t input_kind)
{
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0)
    {
        mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, src, strlen(src), 0);
        qstr source_name = lex->source_name;
        mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);
        mp_obj_t module_fun = mp_compile(&parse_tree, source_name, true);
        mp_call_function_0(module_fun);
        nlr_pop();
    }
    else
    {
        // uncaught exception
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
    }
}
#endif

static char *stack_top;
#if MICROPY_ENABLE_GC
    static char heap[4096];
#endif

/* MicroPython runs as a task under RT-Thread */
#define MP_TASK_STACK_SIZE	(64 * 1024)

void mpy_main(void *parm)
{
    int stack_dummy;
    stack_top = (char *)&stack_dummy;

#if MICROPY_PY_THREAD
    mp_thread_init(rt_thread_self()->stack_addr, MP_TASK_STACK_SIZE / sizeof(uintptr_t));
#endif
    /* Stack limit should be less than real stack size, so we have a */
    /* chance to recover from limit hit. (Limit is measured in bytes) */
    mp_stack_set_top(stack_top);
    mp_stack_set_limit(MP_TASK_STACK_SIZE - 1024);

#if MICROPY_ENABLE_GC
    gc_init(heap, heap + sizeof(heap));
#endif
#if MICROPY_HW_ENABLE_UART_REPLC
    mp_getchar_init();
    mp_putsn_init();
#endif
    mp_init();

    pyexec_friendly_repl();

    mp_deinit();

#if MICROPY_HW_ENABLE_UART_REPLC
    mp_putsn_deinit();
    mp_getchar_deinit();
#endif
}

#if MICROPY_ENABLE_GC
void gc_collect(void)
{
    // WARNING: This gc_collect implementation doesn't try to get root
    // pointers from CPU registers, and thus may function incorrectly.
    void *dummy;
    gc_collect_start();
    gc_collect_root(&dummy, ((mp_uint_t)stack_top - (mp_uint_t)&dummy) / sizeof(mp_uint_t));
    gc_collect_end();
    gc_dump_info(&mp_plat_print);
}
#endif

mp_lexer_t *mp_lexer_new_from_file(qstr filename)
{
    mp_raise_OSError(MP_ENOENT);
}

mp_import_stat_t mp_import_stat(const char *path)
{
    return MP_IMPORT_STAT_NO_EXIST;
}

void nlr_jump_fail(void *val)
{
    mp_printf(MICROPY_ERROR_PRINTER, "nlr_jump_fail\n");
    mp_obj_print_exception(&mp_plat_print, MP_OBJ_FROM_PTR(val));
    while (1);
}

void NORETURN __fatal_error(const char *msg)
{
    while (1)
    {
        ;
    }
}

int DEBUG_printf(const char *format, ...)
{
    static char log_buf[512];
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    /* must use vprintf to print */
    rt_vsprintf(log_buf, format, args);
    mp_printf(&mp_plat_print, "%s", log_buf);
    va_end(args);

    return 0;
}

#if defined(RT_USING_FINSH) && defined(FINSH_USING_MSH)
#include <finsh.h>
static void python(uint8_t argc, char **argv)
{
    rt_thread_t tid;

    tid = rt_thread_create("mpy", mpy_main, RT_NULL,
                           4096, 22, 20);
    RT_ASSERT(tid != RT_NULL);
    rt_thread_startup(tid);
}
MSH_CMD_EXPORT(python, MicroPython: `python [file.py]` execute python script);
#endif /* defined(RT_USING_FINSH) && defined(FINSH_USING_MSH) */
