/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                        Copyright (c) James Pearman                          */
/*                                 2013, 2014                                  */
/*                            All Rights Reserved                              */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*    Module:     vexiq.cpp                                                    */
/*    Author:     James Pearman                                                */
/*    Created:    18 Dec 2013                                                  */
/*                                                                             */
/*    Revisions:                                                               */
/*                V1.00  5 March 2014 - First public release                   */
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
/*    Description:                                                             */
/*                                                                             */
/*    VEX IQ sensor simulation                                                 */
/*-----------------------------------------------------------------------------*/
/*                                                                             */

#include <stdio.h>
#include <stdlib.h>
#include <arduino.h>
#include <Wire.h>
#include "pins_arduino.h"
#include "vexiq.h"

static  int  iqState    = 0;
static  int  iqRegister = IQ_VENDOR_ADDR;
static  int  iqEnablePin = 0;

static  iqTouchLedData  MyIqData;

//#define ENABLE_PIN_DEBUG  PIN_PD3

/*-----------------------------------------------------------------------------*/

unsigned char
iqGetRedValue()
{
    return( MyIqData.red );
}
unsigned char
iqGetGreenValue()
{
    return( MyIqData.green );
}
unsigned char
iqGetBlueValue()
{
    return( MyIqData.blue );
}
unsigned char
iqGetCustomValue()
{
    // use the register at address 0x30
    // it's just a demo !
    return( MyIqData.regData.buf[0x30] );
}

/*-----------------------------------------------------------------------------*/
/* initialize the iq sensor data                                               */
/* In this demo we are a touch LED                                             */
/*-----------------------------------------------------------------------------*/

void
IqInit( int enablePin )
{
    int  i;
  
    for(i=0;i<sizeof(MyIqData.regData.buf);i++)
      MyIqData.regData.buf[i] = 0; 
  
    memcpy(MyIqData.regData.named.version, "V1.02.00", 8);
    memcpy(MyIqData.regData.named.vendor,  "VEX IQ  ", 8);
    memcpy(MyIqData.regData.named.devid,   "228-3010        ", 16); // part number is device id

    MyIqData.regData.named.status[0] = 2; // Version number touch led
    MyIqData.regData.named.status[1] = 1; // 
    MyIqData.regData.named.status[2] = 3; // Touch LED
    MyIqData.regData.named.status[3] = 0; // device status
  
    MyIqData.initialized = 0;
 
    // initialize all colors off
    MyIqData.red     = 0;
    MyIqData.green   = 0;
    MyIqData.blue    = 0;

    // I2C enable pin from VexIQ
    pinMode( enablePin, INPUT_PULLUP);
    delay(1);
    
#ifdef ENABLE_PIN_DEBUG
    pinMode(ENABLE_PIN_DEBUG, OUTPUT);
    digitalWrite( ENABLE_PIN_DEBUG, 1 );
#endif

    // Save enable pin
    iqEnablePin = enablePin;
    
    // Block waiting for I2C enable to be high
    // Thsi can happen if the IQ is turned off when we are already powered
    while( digitalRead( iqEnablePin ) == 0 )
      ;

    // Block until enabled
    IqEnablePinInitialCheck();

    // Switch over to default address
    Wire.begin(IQ_DEFAULT_ADDRESS>>1);
    // set twi slave address set TWGCE
    TWAR = IQ_DEFAULT_ADDRESS | 1;
                    
    // Now attach interrupt
    attachInterrupt(digitalPinToInterrupt( enablePin ), IqEnablePinInterrupt, FALLING);
}

/*-----------------------------------------------------------------------------*/
/* wait for I2C enable to toggle                                               */
/*-----------------------------------------------------------------------------*/
void
IqEnablePinInitialCheck()
{
    do
        {
        // Block waiting for I2C enable to change
        while( digitalRead( iqEnablePin )  )
            ;

        } while( IqEnablePinDebounce() ); 
}

/*-----------------------------------------------------------------------------*/
/* debounce I2C enable pin                                                     */
/*-----------------------------------------------------------------------------*/
int
IqEnablePinDebounce()
{
    int debounce = 0;

#ifdef ENABLE_PIN_DEBUG
    digitalWrite( ENABLE_PIN_DEBUG, 0 );
#endif

    // simple debounce, arduino is slow so we don't use the loop from the MSP430 code
    // two digital reads takes about 30uS on the 8MHz Pro I'm using
    if( digitalRead( iqEnablePin ) )
      debounce = 1;
    if( digitalRead( iqEnablePin ) )
      debounce = 1;

#ifdef ENABLE_PIN_DEBUG
    digitalWrite( ENABLE_PIN_DEBUG, 1 );
#endif
    return( debounce );
}

/*-----------------------------------------------------------------------------*/
/* debounce I2C enable pin                                                     */
/*-----------------------------------------------------------------------------*/
void
IqEnablePinInterrupt()
{
    if( !IqEnablePinDebounce() ) {
      // join i2c bus with IQ_INITIAL_ADDRESS
      Wire.begin(IQ_DEFAULT_ADDRESS>>1);       
      // set twi slave address set TWGCE
      TWAR = IQ_DEFAULT_ADDRESS | 1;
    }
}

/*-----------------------------------------------------------------------------*/
/* check to see if we are initialized                                          */
/*-----------------------------------------------------------------------------*/

int
IqInitCheck()
{
    return( MyIqData.initialized );      
}

/*-----------------------------------------------------------------------------*/
/* reset the state machine - not used in this version                          */
/*-----------------------------------------------------------------------------*/

void
IqStateReset()
{
    iqState = 0;
}

/*-----------------------------------------------------------------------------*/
/*  Called with character from receiveEvert                                    */
/*-----------------------------------------------------------------------------*/

void
IqI2CReceived( unsigned char c )
{
     int i;
          
     switch(iqState)
         {
         case  0:
             switch( c )
                 {
                 case  IQ_SETDEV_REG:
                     iqState = 1;
                     break;
                     
                 case  IQ_RESET_REG:
                     iqState = 3;
                     break;
                     
                 default:
                     // all other registers, limit to 127
                     if( c < 0x80 )
                       iqRegister = c;
                     iqState = 2;
                     break;
                 }     
             break;
         
         case  1:
             // change address
             Wire.begin(c>>1);
             TWAR = c | 1;
             printf("address changed to %02X\n", c);

             iqState = 0;
             MyIqData.initialized = 1;
             break;
         
         case  2:
            // write data
            if( iqRegister < 128 )
                 {
                 MyIqData.regData.buf[ iqRegister ] = c;
                 iqRegister++;
                 }
             
             // update local variables
             MyIqData.red   = MyIqData.regData.named.data[0];
             MyIqData.green = MyIqData.regData.named.data[1];
             MyIqData.blue  = MyIqData.regData.named.data[2];
             break;
         
         // Reset detect part 2
         case  3:
             if( c == 0xCA )
                iqState++;             
             break;    
             
         // Reset detect part 3
         case  4:
             if( c == 0x03 )
                 {
                 printf("reset\n");
                 
                 // join i2c bus with DEFAULT_DEVICE
                 Wire.begin(IQ_INITIAL_ADDRESS>>1); 
                 
                 // clear local data
                 MyIqData.initialized = 0;
                 }
                 
             break;    
     
         default:
             break;
         }
}


/*---------------------------------------------------------------------------*/
/*  Called on request event                                                  */
/*---------------------------------------------------------------------------*/

int
IqI2CRequest( unsigned char *buf )
{
    unsigned char *p = buf;
    unsigned char *q = NULL;
    int   i;
    int   ret = 0;
    static int msgCount = 0;
    
    msgCount++;

    // Update register memory
    // we may have changed the local data
    MyIqData.regData.named.data[0] = MyIqData.red;
    MyIqData.regData.named.data[1] = MyIqData.green;
    MyIqData.regData.named.data[2] = MyIqData.blue;

    // Copy data, up to TWI_BUFFER_LENGTH bytes
    q = &MyIqData.regData.buf[ iqRegister ];
   
    for(i=0, ret=0;i<BUFFER_LENGTH;i++)
       {
       ret++;
       *p++ = *q++;
       iqRegister++;
     
       if( iqRegister > 127 )
           break;
       }
   
    // ret bytes of return data
    return(ret);  
}

