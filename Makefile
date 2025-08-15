# Makefile — Simekip (ATmega16U2) + LUFA
# LUFA klasörü workflow tarafından ./lufa altına konuyor.

LUFA_PATH = ./lufa
MCU       = atmega16u2
F_CPU     = 16000000UL
TARGET    = Simekip

SRC       = SimekipHID.c Descriptors.c
CC        = avr-gcc
OBJCOPY   = avr-objcopy
SIZE      = avr-size

# Not: USE_STATIC_OPTIONS ve benzeri makrolar Makefile'da TANIMLANMIYOR.
# Hepsi LUFAConfig.h içinde #define ile veriliyor. Böylece shell parantez/pipe hatası yok.

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -std=gnu99 -Os -ffunction-sections -fdata-sections -Wall -Wextra \
         -I. -I$(LUFA_PATH) -I$(LUFA_PATH)/LUFA \
         -DUSE_LUFA_CONFIG_HEADER \
         -DNO_DEVICE_REMOTE_WAKEUP=1 -DNO_DEVICE_SELF_POWER=1

LDFLAGS = -Wl,--gc-sections

all: $(TARGET).hex

$(TARGET).elf: $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@
	$(SIZE) --mcu=$(MCU) -C $<

clean:
	rm -f $(TARGET).elf $(TARGET).hex
