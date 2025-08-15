#include "Descriptors.h"

/* 64 buton + 2 hat (2x4 bit) + 8 eksen (16 bit) */
const USB_Descriptor_HIDReport_Datatype_t PROGMEM JoystickReport[] = {
  /* Usage Page (Generic Desktop), Usage (Joystick) */
  HID_RI_USAGE_PAGE(8, 0x01),                // Generic Desktop
  HID_RI_USAGE(8, 0x04),                     // Joystick
  HID_RI_COLLECTION(8, 0x01),                // Application

    /* Buttons: 64 x 1bit */
    HID_RI_USAGE_PAGE(8, 0x09),              // Button
    HID_RI_USAGE_MINIMUM(8, 0x01),
    HID_RI_USAGE_MAXIMUM(8, 0x40),           // 64
    HID_RI_LOGICAL_MINIMUM(8, 0x00),
    HID_RI_LOGICAL_MAXIMUM(8, 0x01),
    HID_RI_REPORT_SIZE(8, 0x01),
    HID_RI_REPORT_COUNT(8, 0x40),
    HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),

    /* 2 adet Hat Switch (4+4 bit) */
    HID_RI_USAGE_PAGE(8, 0x01),              // Generic Desktop
    HID_RI_USAGE(8, 0x39),                   // Hat switch
    HID_RI_USAGE(8, 0x39),                   // Hat switch (2.)
    HID_RI_LOGICAL_MINIMUM(8, 0x00),
    HID_RI_LOGICAL_MAXIMUM(8, 0x07),
    HID_RI_PHYSICAL_MINIMUM(8, 0x00),
    HID_RI_PHYSICAL_MAXIMUM(8, 0x07),
    HID_RI_REPORT_SIZE(8, 0x04),
    HID_RI_REPORT_COUNT(8, 0x02),
    HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),

    /* 8 eksen: X,Y,Z,Rx,Ry,Rz, 0x32,0x33 (16 bit signed) */
    HID_RI_USAGE(8, 0x30),  // X
    HID_RI_USAGE(8, 0x31),  // Y
    HID_RI_USAGE(8, 0x32),  // Z
    HID_RI_USAGE(8, 0x33),  // Rx
    HID_RI_USAGE(8, 0x34),  // Ry
    HID_RI_USAGE(8, 0x35),  // Rz
    HID_RI_USAGE(8, 0x36),  // Slider
    HID_RI_USAGE(8, 0x37),  // Dial
    HID_RI_LOGICAL_MINIMUM(16, 0x8000),      // -32768
    HID_RI_LOGICAL_MAXIMUM(16, 0x7FFF),      //  32767
    HID_RI_REPORT_SIZE(8, 16),
    HID_RI_REPORT_COUNT(8, 8),
    HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),

  HID_RI_END_COLLECTION(0),
};
