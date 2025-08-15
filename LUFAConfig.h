#ifndef _LUFA_CONFIG_H_
#define _LUFA_CONFIG_H_

/* AVR8 için temel/ güvenli ayarlar */
#if (ARCH == ARCH_AVR8)
  #define USB_DEVICE_ONLY
  #define USE_STATIC_OPTIONS (USB_DEVICE_OPT_FULLSPEED | USB_OPT_REG_ENABLED | USB_OPT_AUTO_PLL)
  #define FIXED_CONTROL_ENDPOINT_SIZE 8
  #define FIXED_NUM_CONFIGURATIONS 1

  /* 16U2'de USB clock = CPU clock (prescale=1) → 16 MHz */
  #ifndef F_CPU
  #define F_CPU 16000000UL
  #endif
  #define F_USB F_CPU
#endif

#endif
