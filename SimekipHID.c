#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "Descriptors.h"
#include <LUFA/Drivers/USB/USB.h>

typedef struct __attribute__((packed)) {
  uint8_t  buttons[8];   // 64 bit
  uint8_t  hats;         // 2x4 bit
  int16_t  axes[8];      // 8*16 bit
} InputReport_t;

static USB_ClassInfo_HID_Device_t HID_Interface = {
  .Config = {
    .InterfaceNumber = 0,
    .ReportINEndpoint = { .Address = (ENDPOINT_DIR_IN | 1), .Size = 32, .Banks = 2 },
    .PrevReportINBuffer = NULL, .PrevReportINBufferSize = 0,
  },
};

static InputReport_t g_last = {0};

static void SetupHardware(void) {
  MCUSR = 0; wdt_disable(); clock_prescale_set(clock_div_1);
  USB_Init();
}

void EVENT_USB_Device_ConfigurationChanged(void) {
  HID_Device_ConfigureEndpoints(&HID_Interface);
  USB_Device_EnableSOFEvents();
}

void EVENT_USB_Device_ControlRequest(void) {
  HID_Device_ProcessControlRequest(&HID_Interface);
}

void EVENT_USB_Device_StartOfFrame(void) {
  HID_Device_MillisecondElapsed(&HID_Interface);
}

bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData, uint16_t* const ReportSize)
{
  (void)HIDInterfaceInfo; (void)ReportID;
  if (ReportType == HID_REPORT_ITEM_In) {     // <— doğru sabit
    InputReport_t* r = (InputReport_t*)ReportData;
    *r = g_last;
    *ReportSize = sizeof(InputReport_t);
    return true;
  }
  return false;
}

/* Descriptor router + stringler */
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue, const uint8_t wIndex,
                                    const void** const DescriptorAddress)
{
  const uint8_t DescriptorType   = (wValue >> 8);
  const uint8_t DescriptorNumber = (wValue & 0xFF);

  static const USB_Descriptor_Device_t PROGMEM DeviceDescriptor = {
    .Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},
    .USBSpecification       = VERSION_BCD(2,0,0),
    .Class                  = USB_CSCP_NoDeviceClass,
    .SubClass               = USB_CSCP_NoDeviceSubclass,
    .Protocol               = USB_CSCP_NoDeviceProtocol,
    .Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,
    .VendorID               = 0x03EB,
    .ProductID              = 0x2043,
    .ReleaseNumber          = VERSION_BCD(1,0,0),
    .ManufacturerStrIndex   = 1,
    .ProductStrIndex        = 2,
    .SerialNumStrIndex      = 3,
    .NumberOfConfigurations = 1
  };

  typedef struct {
    USB_Descriptor_Configuration_Header_t Config;
    USB_Descriptor_Interface_t            HID_Intf;
    USB_HID_Descriptor_HID_t              HID_Desc;
    USB_Descriptor_Endpoint_t             HID_InEp;
  } USB_Descriptor_Configuration_t_Impl;

  static USB_Descriptor_Configuration_t_Impl C;

  if (DescriptorType == DTYPE_Device) {
    *DescriptorAddress = &DeviceDescriptor;
    return sizeof(DeviceDescriptor);
  }
  else if (DescriptorType == DTYPE_Configuration) {
    C.Config.Header.Size            = sizeof(C);
    C.Config.Header.Type            = DTYPE_Configuration;
    C.Config.TotalConfigurationSize = sizeof(C);
    C.Config.TotalInterfaces        = 1;
    C.Config.ConfigurationNumber    = 1;
    C.Config.ConfigurationStrIndex  = NO_DESCRIPTOR;
    C.Config.ConfigAttributes       = (USB_CONFIG_ATTR_RESERVED | USB_CONFIG_ATTR_SELFPOWERED);
    C.Config.MaxPowerConsumption    = USB_CONFIG_POWER_MA(100);

    C.HID_Intf.Header.Size          = sizeof(USB_Descriptor_Interface_t);
    C.HID_Intf.Header.Type          = DTYPE_Interface;
    C.HID_Intf.InterfaceNumber      = 0;
    C.HID_Intf.AlternateSetting     = 0;
    C.HID_Intf.TotalEndpoints       = 1;
    C.HID_Intf.Class                = HID_CSCP_HIDClass;
    C.HID_Intf.SubClass             = HID_CSCP_NonBootSubclass;
    C.HID_Intf.Protocol             = HID_CSCP_NonBootProtocol;
    C.HID_Intf.InterfaceStrIndex    = NO_DESCRIPTOR;

    C.HID_Desc.Header.Size          = sizeof(USB_HID_Descriptor_HID_t);
    C.HID_Desc.Header.Type          = HID_DTYPE_HID;
    C.HID_Desc.HIDSpec              = VERSION_BCD(1,1,1);
    C.HID_Desc.CountryCode          = 0;
    C.HID_Desc.TotalReportDescriptors = 1;
    C.HID_Desc.HIDReportType        = HID_DTYPE_Report;
    C.HID_Desc.HIDReportLength      = sizeof(JoystickReport);

    C.HID_InEp.Header.Size          = sizeof(USB_Descriptor_Endpoint_t);
    C.HID_InEp.Header.Type          = DTYPE_Endpoint;
    C.HID_InEp.EndpointAddress      = (ENDPOINT_DIR_IN | 1);
    C.HID_InEp.Attributes           = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA);
    C.HID_InEp.EndpointSize         = 32;
    C.HID_InEp.PollingIntervalMS    = 1;

    *DescriptorAddress = &C;
    return sizeof(C);
  }
  else if (DescriptorType == HID_DTYPE_Report) {
    *DescriptorAddress = &JoystickReport;
    return sizeof(JoystickReport);
  }
  else if (DescriptorType == DTYPE_String) {
    static const USB_Descriptor_String_t PROGMEM LanguageString   = USB_STRING_DESCRIPTOR_ARRAY(LANGUAGE_ID_ENG);
    static const USB_Descriptor_String_t PROGMEM ManufacturerString = USB_STRING_DESCRIPTOR(L"Simekip");
    static const USB_Descriptor_String_t PROGMEM ProductString      = USB_STRING_DESCRIPTOR(L"Simekip");
    static const USB_Descriptor_String_t PROGMEM SerialString       = USB_STRING_DESCRIPTOR(L"SIMEKIP-0001");
    switch (DescriptorNumber) {
      case 0x00: *DescriptorAddress = &LanguageString;     return pgm_read_byte(&LanguageString.Header.Size);
      case 0x01: *DescriptorAddress = &ManufacturerString; return pgm_read_byte(&ManufacturerString.Header.Size);
      case 0x02: *DescriptorAddress = &ProductString;      return pgm_read_byte(&ProductString.Header.Size);
      case 0x03: *DescriptorAddress = &SerialString;       return pgm_read_byte(&SerialString.Header.Size);
    }
  }
  return NO_DESCRIPTOR;
}

int main(void) {
  SetupHardware();
  GlobalInterruptEnable();
  for (;;) {
    HID_Device_USBTask(&HID_Interface);
    USB_USBTask();
  }
}
