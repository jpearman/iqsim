iqsim
=====

VEX IQ sensor simulation demo.

Using Sparkfun Arduino Pro/328 8MHz/3.3v board

I2C Wakeup pin is Digital IO 2 (PD2)  
SCK pin is A5  
SDA pin is A4  

The Arduino bootloader takes many mS before it gives control to the user program, this means that an initial
reset command from the IQ roughly 370mS after power on will be missed if it is the only device on the I2C bus.
At this point it looks like the IQ assumes there are no devices plugged in, adding a real sensor on the same
triplet as the Arduino can help with this problem but it's not ideal.

A better way is to either remove or modify the bootloader to jump straight to the user code.  That's what I did for the
demo by checking to see if an input pin was pulled low.  The specific change I added to the bootloader was;

    // Special VEX skip bootloader for 328P board
    #if defined(__AVR_ATmega328P__)
      // Input and pulled high
      VEX_DDR  &= ~_BV(VEX);
      VEX_PORT |= _BV(VEX);

      if(bit_is_clear(VEX_PIN, VEX)) {
          app_start();
      }
    #endif


Where the VEX pin was defined as

    // Pin PB4 used as bootload disable  
    // special version for VexIQ sensor  
    #define VEX_DDR  DDRB  
    #define VEX_PORT PORTB   
    #define VEX_PIN  PINB 
    #define VEX      PINB4   

Add this somewhere near the top of the bootloader, pin 12 (PB4) now needs to be shorted to GND to start user code quickly, if left floating the bootloader will operate as normal.
