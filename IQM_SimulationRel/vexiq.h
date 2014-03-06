/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                        Copyright (c) James Pearman                          */
/*                                 2013, 2014                                  */
/*                            All Rights Reserved                              */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*    Module:     vexiq.h                                                      */
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


#ifndef  _VIQ
#define  _VIQ

#ifdef   __cplusplus
extern "C" {
#endif

// useful arduino pin definitions
#define PIN_PD0     0
#define PIN_PD1     1
#define PIN_PD2     2
#define PIN_PD3     3
#define PIN_PD4     4
#define PIN_PD5     5
#define PIN_PD6     6
#define PIN_PD7     7

#define PIN_PB0     8
#define PIN_PB1     9
#define PIN_PB2     10
#define PIN_PB3     11
#define PIN_PB4     12
#define PIN_PB5     13

#define PIN_PC0     14
#define PIN_PC1     15
#define PIN_PC2     16
#define PIN_PC3     17
#define PIN_PC4     18
#define PIN_PC5     19
#define PIN_PC6     20
#define PIN_PC7     21


#define IQ_DEFAULT_DEVICE   0x60
#define IQ_START_ADDRESS    0x20

#define IQ_VERSION_ADDR     0x00
#define IQ_VENDOR_ADDR      0x08
#define IQ_DEVICEID_ADDR    0x10
#define IQ_STATUS_ADDR      0x20
#define IQ_DATA_ADDR        0x24

#define IQ_SPECIAL_1        0x4A  // probably not used on an iq sensor, ignore
#define IQ_DISABLE_TERM     0x4B  // probably not used on an iq sensor, ignore
#define IQ_ENABLE_TERM      0x4C  // probably not used on an iq sensor, ignore
#define IQ_SETDEV_REG       0x4D
#define IQ_RESET_REG        0x4E

#define IME_BUF_LEN         16

typedef union _iqData {
    struct _var {
      unsigned char  version[8];
      unsigned char  vendor[8];
      unsigned char  devid[16];
      unsigned char  status[4];
      unsigned char  data[92];
      } named;
    
    // Define all data as a 128 byte memory region  
    unsigned char buf[128];
  } iqData;


// Not used in this demo
#define IQ_MOTOR_STATUS    (IQ_STATUS_ADDR + 0x03 )
#define IQ_MOTOR_DATA      (IQ_DATA_ADDR   + 0x0E )

typedef struct _imeData {
    iqData         regData;
    
    unsigned int   initialized;
    unsigned int   term;
    unsigned long  encoder;

} iqMotorData;

// collect the data for a touch LED here
typedef struct _iqTouchLedData {
    iqData         regData;
    
    unsigned int   initialized;

    // just local copies of the register data
    unsigned char  red;
    unsigned char  green;
    unsigned char  blue;
} iqTouchLedData;

unsigned char  iqGetRedValue(void);
unsigned char  iqGetGreenValue(void);
unsigned char  iqGetBlueValue(void);
unsigned char  iqGetCustomValue(void);

void           IqInit(void);
int            IqInitCheck(void);

void           IqI2CReceived( unsigned char c );
int            IqI2CRequest(unsigned char *buf );
void           IqStateReset(void);

#ifdef   __cplusplus
}  // Close scope of extern "C" declaration
#endif

#endif  /* _VIQ */
