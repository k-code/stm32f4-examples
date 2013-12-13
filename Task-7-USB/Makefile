PROJECT_NAME=usb

BUILDDIR = build

DEVICE = inc/STM32F4xx
CORE = inc/CMSIS
PERIPH = inc/STM32F4xx_StdPeriph_Driver
DISCOVERY = inc
USB = inc/STM32_USB_Device_Library
USB_OTG = inc/STM32_USB_OTG_Driver

#SOURCES += $(DISCOVERY)/src/stm32f4_discovery.c

SOURCES += \
			$(PERIPH)/src/misc.c \
			$(PERIPH)/src/stm32f4xx_exti.c \
			$(PERIPH)/src/stm32f4xx_gpio.c \
			$(PERIPH)/src/stm32f4xx_rcc.c \
			$(PERIPH)/src/stm32f4xx_syscfg.c \
			$(USB)/Class/cdc/src/usbd_cdc_core.c \
			$(USB)/Core/src/usbd_core.c \
			$(USB)/Core/src/usbd_ioreq.c \
			$(USB)/Core/src/usbd_req.c \
			$(USB_OTG)/src/usb_core.c \
			$(USB_OTG)/src/usb_dcd.c \
			$(USB_OTG)/src/usb_dcd_int.c \

SOURCES += startup_stm32f4xx.S

SOURCES += \
		src/main.c \
		src/stm32f4xx_it.c \
		src/stm32f4_discovery.c \
		src/system_stm32f4xx.c \
		src/usb_bsp.c \
		src/usbd_cdc.c \
		src/usbd_cdc_vcp.c \
		src/usbd_desc.c \
		src/usbd_usr.c

OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SOURCES))))

INCLUDES += -I$(DEVICE) \
			-I$(CORE) \
			-I$(PERIPH)/inc \
			-I$(DISCOVERY) \
			-I$(USB)/Class/cdc/inc \
			-I$(USB)/Core/inc \
			-I$(USB_OTG)/inc \
			-I.

ELF = $(BUILDDIR)/$(PROJECT_NAME).elf
HEX = $(BUILDDIR)/$(PROJECT_NAME).hex
BIN = $(BUILDDIR)/$(PROJECT_NAME).bin

CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
AR = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
GDB = arm-none-eabi-gdb

CFLAGS  = -O0 -g -Wall -I.\
   -mcpu=cortex-m4 -mthumb \
   -mfpu=fpv4-sp-d16 -mfloat-abi=hard \
   $(INCLUDES) -DUSE_STDPERIPH_DRIVER \
   -DSTM32F4XX -DUSE_STM32F4_DISCOVERY \
   -DHSE_VALUE=8000000 \
   -DUSE_USB_OTG_FS

LDSCRIPT = stm32_flash.ld
LDFLAGS += -T$(LDSCRIPT) -mthumb -mcpu=cortex-m4 -nostdlib

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

$(ELF): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.S
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

flash: $(BIN)
	st-flash write $(BIN) 0x8000000

debug: $(ELF)
	$(GDB) -tui $(ELF)

all: $(HEX) $(BIN)

.PHONY: clean
clean:
	rm -rf build
