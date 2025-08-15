# Makefile — Simekip (ATmega16U2) on GitHub Actions
# LUFA_PATH will be ./lufa (downloaded by the workflow)

LUFA_PATH = ./lufa
MCU=atmega16u2
F_CPU=16000000UL
TARGET=Simekip

SRC = SimekipHID.c Descriptors.c
CC=avr-gcc
OBJCOPY=avr-objcopy
SIZE=avr-size

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -std=gnu99 -Os -ffunction-sections -fdata-sections -Wall -Wextra \
         -I. -I$(LUFA_PATH) -I$(LUFA_PATH)/LUFA \
         -DFIXED_CONTROL_ENDPOINT_SIZE=8 -DFIXED_NUM_CONFIGURATIONS=1 \
         -DNO_DEVICE_REMOTE_WAKEUP=1 -DNO_DEVICE_SELF_POWER=1 -DUSB_DEVICE_ONLY \
         -DUSE_STATIC_OPTIONS=(USB_DEVICE_OPT_FULLSPEED|USB_OPT_REG_ENABLED|USB_OPT_AUTO_PLL)
LDFLAGS = -Wl,--gc-sections

all: $(TARGET).hex

$(TARGET).elf: $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@
	$(SIZE) --mcu=$(MCU) -C $<

clean:
	rm -f $(TARGET).elf $(TARGET).hex
