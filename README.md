# ESP32APA102Driver

Uses the Espressif ESP-IDF to drive APA102 &amp; SK9822 Addressable Pixels via the SPI Port

# Build instructions

  * make menuconfig
    * Select the appropriate Serial port your ESP32 module is connected to
  * make flash
  
# PIN Outs

  Pin 18  : CLOCK PIN
  
  Pin 23  : DATA PIN

# Compatible Pixels
 
This code will work with APA102 and SK9822 pixels, it should work fine with any other clones. 
  
 
