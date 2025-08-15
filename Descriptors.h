#pragma once
#include <avr/pgmspace.h>
#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Drivers/USB/Class/Common/HIDReportData.h>

// Joystick report array tanımı (Descriptors.c içinde tanımlı)
extern const USB_Descriptor_HIDReport_Datatype_t PROGMEM JoystickReport[];

// LUFA'nın beklediği imza (sadece prototype)
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                    const uint8_t wIndex,
                                    const void** const DescriptorAddress)
                                    ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);
