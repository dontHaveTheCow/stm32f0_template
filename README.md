Debuggin:

openocd -f interface/stlink-v2.cfg -f target/stm32f0x_stlink.cfg

arm-none-eabi-gdb

set auto-load local-gdbinit
target remote localhost:3333
monitor arm semihosting enable
set logging file gdb.txt
set logging on
monitor reset halt
load
monitor reset init
continue