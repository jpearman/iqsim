/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                        Copyright (c) James Pearman                          */
/*                                    2014                                     */
/*                            All Rights Reserved                              */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*    Module:     customSensor.c                                               */
/*    Author:     James Pearman                                                */
/*    Created:    5 March 2014                                                 */
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
/*    Just a hack - not production code !!!                                    */
/*    Use one extra "special" register to turn on and off the arduino led      */
/*                                                                             */
/*-----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------*/
/*  Find the first touch led installed                                         */
/*  scan all ports looking for an installed touch LED                          */
/*  return the port number if found else (-1)                                  */
/*-----------------------------------------------------------------------------*/

tSensors
FindTouchLed()
{
    TVexIQDeviceTypes   type;
    TDeviceStatus       status;
    short               ver;
    short               index;

    // Get all device info
    for(index=(short)PORT1;index<=(short)PORT12;index++)
        {
        getVexIqDeviceInfo( index, type, status, ver );
        if( type == vexIQ_SensorLED )
            return((tSensors)index);
        }
    return((tSensors)-1);
}

/*-----------------------------------------------------------------------------*/
/*  Raw I2C, send data to address of port                                      */
/*-----------------------------------------------------------------------------*/

void
setTouchSpecialReg( portName port, int addr, char *data, int len )
{
    TVexIqI2CResults     status;

    // check status of I2C bus
    status = vexIqGetI2CStatus( port );
    while( status != i2cRsltIdleAndOK )
        status = vexIqGetI2CStatus( port );

    // Send the buffer
    StartI2CDeviceBytesWrite( port, addr, data, len );
    wait1Msec(2);
}

/*-----------------------------------------------------------------------------*/
/*  Status screen showing program running time                                 */
/*-----------------------------------------------------------------------------*/

#define LCD_ROW1    43
#define LCD_ROW2    34
#define LCD_ROW3    25
#define LCD_ROW4    16
#define LCD_ROW5     7

void
DisplaySplash()
{
    long secs = nPgmTime/1000;
    int  mins;
    int  hours;

    mins  = secs / 60;
    hours = mins / 60;

    // program running time
    displayStringAt(  8, LCD_ROW1, "%02d:%02d:%02d", hours, mins % 60, secs % 60 );
    displayStringAt(  8, LCD_ROW2, "Custom Sensor Demo");
    displayStringAt(  8, LCD_ROW4, "5 Mar 2014");
    displayStringAt(  8, LCD_ROW5, "J Pearman");
}

/*-----------------------------------------------------------------------------*/

task main()
{
    tSensors ledPort = -1;
    char    data[8];

    // Look for a Touch sensor
    ledPort = FindTouchLed();

    while(true)
        {
        // Set color to black
        // set register 0x30 to 0
        if( ledPort >= PORT1 ) {
            setTouchLEDRGB( ledPort, 0, 0, 0 );
            data[0] = 0;
            setTouchSpecialReg( (portName)ledPort, 0x30, data, 1 );
            }

        // delay
        wait1Msec(250);

        // Set color to red
        // set register 0x30 to 1
        if( ledPort >= PORT1 ) {
            setTouchLEDRGB( ledPort, 255, 0, 0 );
            data[0] = 1;
            setTouchSpecialReg( (portName)ledPort, 0x30, data, 1 );
            }

        // delay
        wait1Msec(250);

        // update running time
        DisplaySplash();
        }
}
