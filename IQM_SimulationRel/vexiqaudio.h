/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                        Copyright (c) James Pearman                          */
/*                                 2013, 2016                                  */
/*                            All Rights Reserved                              */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*    Module:     vexiqaudio.h                                                 */
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

#ifndef  _VIQAUDIO
#define  _VIQAUDIO

#ifdef   __cplusplus
extern "C" {
#endif

void    AudioInitialize();
void    AudioPlayTrack( int number );

#ifdef   __cplusplus
}  // Close scope of extern "C" declaration
#endif

#endif  /* _VIQAUDIO */
