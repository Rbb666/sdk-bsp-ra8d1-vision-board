import os
import sys

# toolchains options
ARCH='arm'
CPU='cortex-m85'
CROSS_TOOL='keil'

if os.getenv('RTT_CC'):
    CROSS_TOOL = os.getenv('RTT_CC')
if os.getenv('RTT_ROOT'):
    RTT_ROOT = os.getenv('RTT_ROOT')

# cross_tool provides the cross compiler
# EXEC_PATH is the compiler execute path, for example, CodeSourcery, Keil MDK, IAR
if  CROSS_TOOL == 'gcc':
    PLATFORM    = 'gcc'
    EXEC_PATH   = r'C:\Users\XXYYZZ'
elif CROSS_TOOL == 'keil':
    PLATFORM    = 'armclang'
    EXEC_PATH   = r'C:/Keil_v5'
elif CROSS_TOOL == 'iar':
    PLATFORM    = 'iccarm'
    EXEC_PATH   = r'C:/Program Files/IAR Systems/Embedded Workbench 8.0'
elif CROSS_TOOL == 'llvm-arm':
    PLATFORM    = 'llvm-arm'
    EXEC_PATH   = r'D:\Progrem\LLVMEmbeddedToolchainForArm-17.0.1-Windows-x86_64\bin'

if os.getenv('RTT_EXEC_PATH'):
    EXEC_PATH = os.getenv('RTT_EXEC_PATH')

BUILD = 'debug' 
# BUILD = 'release' 

if PLATFORM == 'gcc':
    # toolchains
    PREFIX = 'arm-none-eabi-'
    CC = PREFIX + 'gcc'
    AS = PREFIX + 'gcc'
    AR = PREFIX + 'ar'
    CXX = PREFIX + 'g++'
    LINK = PREFIX + 'gcc'
    TARGET_EXT = 'elf'
    SIZE = PREFIX + 'size'
    OBJDUMP = PREFIX + 'objdump'
    OBJCPY = PREFIX + 'objcopy'
    NM = PREFIX + 'nm'

    DEVICE = ' -march=armv8.1-m.main+mve.fp+fp.dp -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections'
    CFLAGS = DEVICE + ' -Dgcc'
    AFLAGS = ' -c' + DEVICE + ' -x assembler-with-cpp -Wa,-mimplicit-it=thumb '
    LFLAGS = DEVICE + ' -Wl,--gc-sections,-Map=rtthread.map,-cref,-u,Reset_Handler -T script/fsp.ld -L script/'

    CPATH = ''
    LPATH = ''

    if BUILD == 'debug':
        CFLAGS += ' -O0 -gdwarf-2 -g -Wall'
        AFLAGS += ' -gdwarf-2'
    else:
        CFLAGS += ' -Os'
    CXXFLAGS = CFLAGS

    POST_ACTION = OBJCPY + ' -O ihex $TARGET rtthread.hex\n' + SIZE + ' $TARGET \n'
    # POST_ACTION += OBJCPY + ' -O binary $TARGET rtthread.bin\n' + SIZE + ' $TARGET \n'

elif PLATFORM == 'armclang':
    # toolchains
    CC = 'armclang'
    CXX = 'armclang'
    AS = 'armasm'
    AR = 'armar'
    LINK = 'armlink'
    TARGET_EXT = 'axf'

    DEVICE = ' --cpu Cortex-M85'

    CFLAGS = ' -mcpu=Cortex-M85 -xc -std=c99 --target=arm-arm-none-eabi -mfpu=neon-fp-armv8 -mfloat-abi=hard -c'
    CFLAGS += ' -fno-rtti -funsigned-char -ffunction-sections'
    CFLAGS += ' -Wno-license-management -Wuninitialized -Wall -Wmissing-declarations -Wpointer-arith -Waggregate-return -Wfloat-equal'

    AFLAGS = DEVICE + ' --apcs=interwork '

    LFLAGS = DEVICE + ' --scatter ' + 'script/fsp.scat'
    LFLAGS +=' --info sizes --info totals --info unused --info veneers '
    LFLAGS += ' --list rt-thread.map --strict'
    LFLAGS += ' --diag_suppress 6319,6314 --summary_stderr --info summarysizes'
    LFLAGS += ' --map --load_addr_map_info --xref --callgraph --symbols'
    LFLAGS += ' --libpath=' + EXEC_PATH + '/ARM/ARMCLANG/lib'

    EXEC_PATH += '/ARM/ARMCLANG/bin/'

    if BUILD == 'debug':
        CFLAGS += ' -g -O0'
        AFLAGS += ' -g'
    else:
        CFLAGS += ' -Os'

    POST_ACTION = 'fromelf --bin $TARGET --output rtthread.bin \nfromelf -z $TARGET \n'
elif PLATFORM == 'llvm-arm':
    # toolchains
    PREFIX = 'llvm-'
    CC = 'clang'
    AS = 'clang'
    AR = PREFIX + 'ar'
    CXX = 'clang++'
    LINK = 'clang'
    TARGET_EXT = 'elf'
    SIZE = PREFIX + 'size'
    OBJDUMP = PREFIX + 'objdump'
    OBJCPY = PREFIX + 'objcopy'
    DEVICE = ' --target=arm-none-eabihf -mfloat-abi=hard -march=armv8.1m.main+fp'
    DEVICE += ' -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti'
    CFLAGS = DEVICE
    CFLAGS += ' -mfloat-abi=hard -march=armv8.1m.main+fp'
    AFLAGS = ' -c' + DEVICE + ' -Wa,-mimplicit-it=thumb ' ## -x assembler-with-cpp
    LFLAGS = DEVICE + ' -Wl,--gc-sections,-Map=rt-thread.map,-u,Reset_Handler -lcrt0 -T script/fsp.ld -L script/'

    CPATH = ''
    LPATH = ''

    if BUILD == 'debug':
        CFLAGS += ' -O0 -gdwarf-2 -g'
        AFLAGS += ' -gdwarf-2'
    else:
        CFLAGS += ' -O2'

    CXXFLAGS = CFLAGS 

    POST_ACTION = OBJCPY + ' -O ihex $TARGET rtthread.hex\n' + SIZE + ' $TARGET \n'

def dist_handle(BSP_ROOT, dist_dir):
    import sys
    cwd_path = os.getcwd()
    sys.path.append(os.path.join(os.path.dirname(BSP_ROOT), 'tools'))
    from sdk_dist import dist_do_building
    dist_do_building(BSP_ROOT, dist_dir)
