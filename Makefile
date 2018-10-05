CURR_DIR = $(shell basename $(CURDIR))
BUILD_DIR = build
PROJECT=$(CURR_DIR)

CCPREFIX = arm-none-eabi-
LD=$(CCPREFIX)cc
CC=$(CCPREFIX)gcc
AR=$(CCPREFIX)ar
AS=$(CCPREFIX)gcc -x assembler-with-cpp
#AS=$(CCPREFIX)as
CP=$(CCPREFIX)objcopy
OD=$(CCPREFIX)objdump
SE=$(CCPREFIX)size

# List all C defines here
DDEFS = -DSTM32F0XX -DUSE_STDPERIPH_DRIVER

SF   =st-flash
MCU  = cortex-m0

ASRC = ./startup/startup_stm32f0xx.s
SRC  = ./src/main.c
SRC += ./src/system_stm32f0xx.c
SRCDIRS := $(shell find . -name '*.c' -exec dirname {} \; | uniq)
ASMDIRS := $(shell find . -name '*.s' -exec dirname {} \; | uniq)

STMSPDDIR    = STM32F0xx_StdPeriph_Driver
STMSPSRCDDIR = $(STMSPDDIR)/src
STMSPINCDDIR = $(STMSPDDIR)/inc

## used parts of the STM-Library
#SRC += $(STMSPSRCDDIR)/stm32f0xx_adc.c
#SRC += $(STMSPSRCDDIR)/stm32f0xx_cec.c
#SRC += $(STMSPSRCDDIR)/stm32f0xx_crc.c
#SRC += $(STMSPSRCDDIR)/stm32f0xx_comp.c
#SRC += $(STMSPSRCDDIR)/stm32f0xx_dac.c
#SRC += $(STMSPSRCDDIR)/stm32f0xx_dbgmcu.c
#SRC += $(STMSPSRCDDIR)/stm32f0xx_dma.c
#SRC += $(STMSPSRCDDIR)/stm32f0xx_exti.c
#SRC += $(STMSPSRCDDIR)/stm32f0xx_flash.c
SRC += $(STMSPSRCDDIR)/stm32f0xx_gpio.c
#SRC += $(STMSPSRCDDIR)/stm32f0xx_syscfg.c
#SRC += $(STMSPSRCDDIR)/stm32f0xx_i2c.c
#SRC += $(STMSPSRCDDIR)/stm32f0xx_iwdg.c
#SRC += $(STMSPSRCDDIR)/stm32f0xx_pwr.c
SRC += $(STMSPSRCDDIR)/stm32f0xx_rcc.c
#SRC += $(STMSPSRCDDIR)/stm32f0xx_rtc.c
SRC += $(STMSPSRCDDIR)/stm32f0xx_spi.c
#SRC += $(STMSPSRCDDIR)/stm32f0xx_tim.c
SRC += $(STMSPSRCDDIR)/stm32f0xx_usart.c
#SRC += $(STMSPSRCDDIR)/stm32f0xx_wwdg.c
SRC += $(STMSPSRCDDIR)/stm32f0xx_misc.c

STARTUP = ./startup/startup_stm32f0xx.s

# List all include directories here
INCDIRS = ./inc ./inc/CMSIS $(STMSPINCDDIR)
              
# List the user directory to look for the libraries here
LIBDIRS += ./inc/user_libs
 
# List all user libraries here
LIBS = spi.c
LIBS += ./inc/user_libs/delay.c
LIBS += ./inc/user_libs/serial.c

# Define optimisation level here
OPT = -Os
 
# Define linker script file here
LINKER_SCRIPT = ./LinkerScript.ld

INCDIR  = $(patsubst %,-I%, $(INCDIRS))
LIBDIR  = $(patsubst %,-L%, $(LIBDIRS))
LIB     = $(patsubst %,-l%, $(LIBS))

## run from Flash
DEFS    = $(DDEFS) -DRUN_FROM_FLASH=1

OBJS  = $(STARTUP:.s=.o) $(SRC:.c=.o) ./inc/user_libs/$(LIBS:.c=.o)

MCFLAGS = -mcpu=$(MCU)
 
ASFLAGS = $(MCFLAGS) -g -gdwarf-2 -mthumb 
CPFLAGS = $(MCFLAGS) $(OPT) -g -gdwarf-2 -mthumb   -fomit-frame-pointer -Wall -Wstrict-prototypes -fverbose-asm $(DEFS) 
LDFLAGS = $(MCFLAGS) -g -gdwarf-2 -mthumb --specs=rdimon.specs -lc -lrdimon -T$(LINKER_SCRIPT) -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map,--cref,--no-warn-mismatch  
#LDFLAGS = $(MCFLAGS) -g -gdwarf-2 -mthumb -lrdimon -nostartfiles -T$(LINKER_SCRIPT) -Wl,-Map=$(PROJECT).map,--cref,--no-warn-mismatch $(LIBDIR) 
#+= --specs=rdimon.specs -lc -lrdimon

#Formatas
COLOR_BEGIN=\033[1;33m
COLOR_END=\033[0m

DEB_COL_BEG=\033[1;31m 
DEB_COL_END=\033[0m

.PHONY: all clean debug prep_release prep_debug release flash_swd flash_uart

ELF_FILE=$(PROJECT).elf
BIN_FILE=$(PROJECT).bin
#
# Debug build settings
#
DBG_CPFLAGS = -Wa,-ahlms=$(BUILD_DIR)/debug/$(<:.c=.lst)
DBG_ASFLAGS = -Wa,-amhls=$(BUILD_DIR)/debug/$(<:.s=.lst)
DBGDIR = $(BUILD_DIR)/debug
DBGELF = $(DBGDIR)/$(ELF_FILE)
DBGBIN = $(DBGDIR)/$(BIN_FILE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBG_LDFLAGS = --specs=rdimon.specs -lc -lrdimon

#
# Release build settings
#
REL_CPFLAGS = -Wa,-ahlms=$(BUILD_DIR)/release/$(<:.c=.lst)
REL_ASFLAGS = -Wa,-amhls=$(BUILD_DIR)/release/$(<:.s=.lst)
RELDIR = $(BUILD_DIR)/release
RELELF = $(RELDIR)/$(ELF_FILE)
RELBIN = $(RELDIR)/$(BIN_FILE)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
REL_LDFLAGS = --specs=rdimon.specs -lc -lrdimon

# Default build
all: release

#
# Debug rules
#
debug: prep_debug $(DBGBIN)
	@echo "$(DEB_COL_BEG)[DEBUG]$(DEB_COL_END)"
	$(TRGT)size $(DBGELF)

$(DBGBIN): $(DBGELF)
	@echo "$(COLOR_BEGIN) >>>  Generating raw binary file... $(COLOR_END)"
	$(CP) -O binary -S  $< $@

$(DBGELF): $(DBGOBJS)
	@echo "$(COLOR_BEGIN) >>>  Linking $< into $@ file... $(COLOR_END)"
	$(CC) $(DBGOBJS) $(LDFLAGS) -o $@

$(DBGDIR)/%.o: %.c
	@echo "$(COLOR_BEGIN) >>>  Compiling $< into $@ file$(COLOR_END)"
	$(CC) -c $(CPFLAGS) $(DBG_CPFLAGS) -I . $(INCDIR) -I$(LIBDIRS) $< -o $@

$(DBGDIR)/%.o: %.s
	@echo "$(COLOR_BEGIN) >>>  Compiling $< into $@ file ... $(COLOR_END)"
	$(AS) -c $(ASFLAGS) $(DBG_ASFLAGS) $< -o $@
	@echo "$(DEB_COL_BEG) >>>  Printing variables... $(DEB_COL_END)"

#
# Release rules
#
release: prep_release $(RELBIN)
	@echo "$(COLOR_BEGIN) [RELEASE] $(COLOR_END)"
	$(TRGT)size $(RELELF)

$(RELBIN): $(RELELF)
	@echo "$(COLOR_BEGIN) >>>  Generating raw binary file... $(COLOR_END)"
	$(CP) -O binary -S  $< $@

$(RELELF): $(RELOBJS)
	@echo "$(COLOR_BEGIN) >>>  Linking into $@ file... $(COLOR_END)"
	$(CC) $(RELOBJS) $(LDFLAGS) -o $@

$(RELDIR)/%.o: %.c
	@echo "$(COLOR_BEGIN) >>>  Compiling source $< into $@$(COLOR_END)"
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) -I$(LIBDIRS) $< -o $@

$(RELDIR)/%.o: %.s
	@echo "$(COLOR_BEGIN) >>>  Compiling ASM file $< into $@$(COLOR_END)"
	$(AS) -c $(ASFLAGS) $< -o $@
	@echo "$(DEB_COL_BEG) >>>  Printing variables... $(DEB_COL_END)"

#
# Flashing rules
#
flash_swd: release
	@echo "$(COLOR_BEGIN) >>>  Flashing... $(COLOR_END)"
	st-flash write $(RELBIN) 0x8000000

flash_debug: debug
	@echo "$(DEB_COL_BEG) >>>  Flashing... $(DEB_COL_END)"
	st-flash write $(DBGBIN) 0x8000000
	@echo "$(DEB_COL_BEG) >>>  Starting openocd... $(DEB_COL_END)"
	xterm 'openocd -f interface/stlink-v2.cfg -f target/stm32f0x_stlink.cfg' &
	@echo "$(DEB_COL_BEG) >>>  Starting gdb... $(DEB_COL_END)"
	sleep 1
	$(GDB) $(DBGELF)

flash_uart: release
	@echo "$(COLOR_BEGIN) >>>  Flashing ... $(COLOR_END)"
	st-flash write $(RELBIN) 0x8000000

erase:
	@echo "$(COLOR_BEGIN) >>>  Erasing flash memory ... $(COLOR_END)"
	st-flash erase

#
# Other rules
#
prep_release:
	mkdir -p $(SRCDIRS:./%=build/release/%)
	mkdir -p $(ASMDIRS:./%=build/release/%)

prep_debug:
	mkdir -p $(SRCDIRS:./%=build/debug/%)
	mkdir -p $(ASMDIRS:./%=build/debug/%)

clean:
	rm -r $(BUILD_DIR)
