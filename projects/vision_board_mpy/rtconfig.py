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
    EXEC_PATH   = r'E:/software/keil/keil_Core'
elif CROSS_TOOL == 'iar':
    PLATFORM    = 'iccarm'
    EXEC_PATH   = r'C:/Program Files/IAR Systems/Embedded Workbench 8.0'
elif CROSS_TOOL == 'llvm-arm':
    PLATFORM    = 'llvm-arm'
    EXEC_PATH   = r'C:\Renesas\RA\e2studio_v2024-04_fsp_v5.4.0\toolchains\llvm_arm\LLVM-ET-Arm-18.1.3-Windows-x86_64\bin'

if os.getenv('RTT_EXEC_PATH'):
    EXEC_PATH = os.getenv('RTT_EXEC_PATH')

# BUILD = 'debug' 
BUILD = 'release' 

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
    CFLAGS = DEVICE + ' -flax-vector-conversions -fshort-enums -fno-unroll-loops'
    AFLAGS = ' -c' + DEVICE + ' -x assembler-with-cpp -Wa,-mimplicit-it=thumb'
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
    AS = 'armclang'
    AR = 'armar'
    LINK = 'armlink'
    TARGET_EXT = 'axf'

    DEVICE = ' --cpu cortex-m85+pacbti'

    CFLAGS = ' --target=arm-arm-none-eabi -mcpu=cortex-m85 -xc -std=c99 -mfpu=neon-fp-armv8 -mfloat-abi=hard -c -gdwarf-4'
    CFLAGS += ' -fno-rtti -funsigned-char -ffunction-sections -flax-vector-conversions -fno-vectorize -fno-unroll-loops'
    CFLAGS += ' -Wno-license-management -Wuninitialized -Wall -Wmissing-declarations -Wpointer-arith -Waggregate-return -Wfloat-equal'

    AFLAGS = '--target=arm-arm-none-eabi -mcpu=cortex-m85 -mfloat-abi=hard -masm=gnu -c -gdwarf-4'

    LFLAGS = DEVICE + '--fpu=FPv5_D16 --info sizes --info totals --info unused --info veneers '
    LFLAGS += ' --summary_stderr --info summarysizes --map --load_addr_map_info --xref --callgraph --symb '
    LFLAGS += ' --list rt-thread.map '
    LFLAGS += r' --strict --scatter ".\script\fsp.scat" --diag_suppress 6319,6314 '
    CFLAGS += ' -I' + EXEC_PATH + '/ARM/ARMCLANG/include'
    LFLAGS += ' --libpath=' + EXEC_PATH + '/ARM/ARMCLANG/lib'

    EXEC_PATH += '/ARM/ARMCLANG/bin/'

    if BUILD == 'debug':
        CFLAGS += ' -g -O0'
        AFLAGS += ' -g'
    else:
        CFLAGS += ' -Omax -flto'
    CXXFLAGS = CFLAGS

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
    DEVICE = ' --target=arm-none-eabihf -mfloat-abi=hard -march=armv8.1m.main+fp -mthumb'
    DEVICE += ' -fno-vectorize -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti'
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
