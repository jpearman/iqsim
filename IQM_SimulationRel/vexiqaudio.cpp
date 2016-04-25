/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                        Copyright (c) James Pearman                          */
/*                                 2013, 2016                                  */
/*                            All Rights Reserved                              */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*    Module:     vexiqaudio.cpp                                               */
/*    Author:     James Pearman                                                */
/*    Created:    25 April 2016                                                */
/*                                                                             */
/*    Revisions:                                                               */
/*                V1.00 25 April 2016 - First public release                   */
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
/*    Wrapper for audio shield functionality                                   */
/*-----------------------------------------------------------------------------*/
/*                                                                             */

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
#include "vexiqaudio.h"

// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET   -1      // VS1053 reset pin (unused!)
#define SHIELD_CS       7      // VS1053 chip select pin (output)
#define SHIELD_DCS      6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  //Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

void
AudioInitialize()
{
  printf("Adafruit VS1053 Library Setup\n");

  // initialise the music player
  if (! musicPlayer.begin()) { // initialise the music player
     printf("Couldn't find VS1053, do you have the right pins defined?\n");
     return;
  }
  printf("VS1053 found\n");

  musicPlayer.sineTest(0x44, 500);    // Make a tone to indicate VS1053 is working
 
  if (!SD.begin(CARDCS)) {
    printf("SD failed, or not present\n");
    return;  // don't do anything more
  }
  printf("SD OK!\n");
    
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20,20);

  /***** Two interrupt options! *******/ 
  // This option uses timer0, this means timer1 & t2 are not required
  // (so you can use 'em for Servos, etc) BUT millis() can lose time
  // since we're hitchhiking on top of the millis() tracker
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT);
  
  // This option uses a pin interrupt. No timers required! But DREQ
  // must be on an interrupt pin. For Uno/Duemilanove/Diecimilla
  // that's Digital #2 or #3
  // See http://arduino.cc/en/Reference/attachInterrupt for other pins
  // *** This method is preferred
  if (! musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT))
    printf("DREQ pin is not an interrupt pin\n");
}

void
AudioPlayTrack( int number )
{
  char    track[16];

  // tracks < 0 and > 254 are invalid
  if( number < 0 || number >= 255 )
    return;
    
  sprintf(track,"track%03d.mp3", number );

  // Stop any music that is playing
  if(musicPlayer.playingMusic) {
    musicPlayer.stopPlaying();
  }

  // track 0 is used to stop playing
  if( number == 0 )
    return;
    
  // Start playing a file, then we can do stuff while waiting for it to finish
  if (! musicPlayer.startPlayingFile(track)) {
    printf("Could not open file %s\n", track );
    return;
  }
  
  printf("Started playing %s\n", track);
}

