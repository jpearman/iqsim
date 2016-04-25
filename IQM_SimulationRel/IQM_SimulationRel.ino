/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                        Copyright (c) James Pearman                          */
/*                                 2013, 2014                                  */
/*                            All Rights Reserved                              */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*    Module:     IQM_Simulation                                               */
/*    Author:     James Pearman                                                */
/*    Created:    12 Dec 2013                                                  */
/*                                                                             */
/*    Revisions:                                                               */
/*                V1.00  5 March 2014 - First public release                   */
/*                V1.01  3 April 2016 - Update for wakeup pulse                */
/*                V1.02  25 April 2016 - Support for VS1053 autio shield       */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*    Licensed under the Apache License, Version 2.0 (the "License");          */
/*    you may not use this file except in compliance with the License.         */
/*    You may obtain a copy of the License at                                  */
/*                                                                             */
/*      http://www.apache.org/licenses/LICENSE-2.0                             */
/*                                                                             */
/*    Unless required by applicable law or agreed to in writing, software      */
/*    distributed under the License is distributed on an "AS IS" BASIS,        */
/*    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/*    See the License for the specific language governing permissions and      */
/*    limitations under the License.                                           */
/*                                                                             */
/*    The author can be contacted on the vex forums as jpearman                */
/*    or electronic mail using jbpearman_at_mac_dot_com                        */
/*    Mentor for team 8888 RoboLancers, Pasadena CA.                           */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*    Just a hack - not production code !!!                                    */
/*    Simulate a touch LED                                                     */
/*    Use one extra "special" register to turn on and off the arduino led      */
/*                                                                             */
/*-----------------------------------------------------------------------------*/


#include <Wire.h>
#include "pins_arduino.h"
#include "vexiq.h"
#include "vexiqaudio.h"

// standard output for debug
static FILE uartout = {0} ;

// create a output function
// This works because Serial.write, although of
// type virtual, already exists.
static int uart_putchar (char c, FILE *stream)
{
    Serial.write(c) ;
    return 0 ;
}

// Arduino setup function

void setup()
{     
     // standard arduino LED - now used by audio shield
     //pinMode(PIN_PB5, OUTPUT);

     Serial.begin(19200);           // start serial for output
   
     // fill in the UART file descriptor with pointer to writer.
     fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);

     // The uart is the standard output device STDOUT.
     stdout = &uartout ;
     printf("VEX IQ Touch LED simulation\n");

     // Init audio board
     AudioInitialize();

     // join i2c bus with IQ_INITIAL_ADDRESS
     IqSetupI2C( IQ_INITIAL_ADDRESS );
     
     // set the I2C callbacks
     Wire.onReceive(receiveEvent); // register event
     Wire.onRequest(requestEvent); // register event

     // Init the sensor simulation
     IqInit( PIN_PD2 );
     
     // led on     
     //digitalWrite( PIN_PB5, 0);

}

// 
void loop()
{
    // red value is used to start track playback
    // green value is the track number
    if( iqGetRedValue() > 0 ) {
        int trackNumber = iqGetGreenValue();
        iqSetRedValue(0);
        printf("play %03d\n", trackNumber );
        AudioPlayTrack( trackNumber );
      }
    
    // small delay
    delay(4);    
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
    while(Wire.available()) // loop through all characters
        IqI2CReceived( (unsigned char)Wire.read() );

    IqStateReset();  
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent()
{
    int  len, lensent;
    uint8_t buf[BUFFER_LENGTH];
    
    // create reply
    len = IqI2CRequest( (unsigned char *)buf );
  
    // Send to I2C
    if(len > 0)
      lensent = Wire.write(buf, len);            
}


