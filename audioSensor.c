/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                        Copyright (c) James Pearman                          */
/*                                    2014                                     */
/*                            All Rights Reserved                              */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*    Module:     audioSensor.c                                                */
/*    Author:     James Pearman                                                */
/*    Created:    25 April 2016                                                */
/*                                                                             */
/*    Revisions:                                                               */
/*                V1.00  25 April 2016 - First public release                  */
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
/*    Red LED value is used to trigger play                                    */
/*    Green led value is used for track number                                 */
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
/*  Status screen showing program running time and track                       */
/*-----------------------------------------------------------------------------*/

#define LCD_ROW1    43
#define LCD_ROW2    34
#define LCD_ROW3    25
#define LCD_ROW4    16
#define LCD_ROW5     7

void
DisplaySplash( int track )
{
    long secs = nPgmTime/1000;
    int  mins;
    int  hours;
    char str[32];

    mins  = secs / 60;
    hours = mins / 60;

    // program running time
    displayStringAt(  8, LCD_ROW1, "%02d:%02d:%02d", hours, mins % 60, secs % 60 );
    displayStringAt(  8, LCD_ROW2, "Audio Shield Demo");
		sprintf(str, "Track %03d", track);
    displayStringAt(  8, LCD_ROW3, str);
    displayStringAt(  8, LCD_ROW4, "25 Apr 2016");
    displayStringAt(  8, LCD_ROW5, "J Pearman");
}

/*-----------------------------------------------------------------------------*/

task main()
{
    tSensors ledPort = -1;
		int select     = 0;
		int old_select = 0;
		
    // Look for a Touch sensor
    ledPort = FindTouchLed();

    while(true)
        {
				if( nLCDButtons == kButtonDown  )
					if( select++ == 10 ) select = 0;
		
				if( nLCDButtons == kButtonUp  )
					if( select-- == 0 ) select = 9;

        if( select != old_select ) {
        	if( ledPort >= PORT1 )
	          setTouchLEDRGB( ledPort, 1, select, 0 );
      		old_select = select;
      	}

        // Wait for LCD button release
        while( nLCDButtons != kButtonNone )
          wait1Msec(10);

        // delay
        wait1Msec(100);

        // update running time
        DisplaySplash( select );
        }
}