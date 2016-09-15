#include "UsbFat.h"
//------------------------------------------------------------------------------
bool initUSB(USB* usb) {
 uint8_t last_state = 0;
 uint8_t current_state = 0;
  uint32_t m = millis();
  for (uint8_t i = 0; usb->Init(1000) == -1; i++) {
    if (USB_FAT_DBG_MODE) {
      SerialUSB.println(F("No USB HOST Shield?"));
    }
    if (i > 10) {
      return false;
    }
  }
#if USB_FAT_DBG_MODE
  SerialUSB.print(F("Host initialized, ms: "));
  SerialUSB.println(millis() - m);
#endif  // USB_FAT_DBG_MODE

  usb->vbusPower(vbus_on);
#if USB_FAT_DBG_MODE  
  SerialUSB.print(F("USB powered, ms: "));
  SerialUSB.println(millis() - m);
#endif  // USB_FAT_DBG_MODE
  
  while ((millis() - m) < TIMEOUT_MILLIS) {
    usb->Task();  
    current_state = usb->getUsbTaskState();
#if USB_FAT_DBG_MODE    
    if (last_state != current_state) {
      SerialUSB.print(F("USB state: "));
      SerialUSB.print(current_state, HEX);
      SerialUSB.print(F(", ms: "));
      SerialUSB.println(millis() - m);
    }
    last_state = current_state;
#endif  // USB_FAT_DBG_MODE    
    if(current_state == USB_STATE_RUNNING) {
      return true;
    }
  }
  return false;
}
