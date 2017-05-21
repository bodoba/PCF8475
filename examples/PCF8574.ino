/* *********************************************************************************** */
/*                                                                                     */
/*  Example usage of PCF8874 IO Extender class                                         */
/*                                                                                     */
/*  Copyright (c) 2017 by Bodo Bauer <bb@bb-zone.com>                                  */
/*                                                                                     */
/*  Thanks to Rob Tillaart for his initial I2C PCF8574 library for Arduino             */
/*                                                                                     */
/*  This program is free software: you can redistribute it and/or modify               */
/*  it under the terms of the GNU General Public License as published by               */
/*  the Free Software Foundation, either version 3 of the License, or                  */
/*  (at your option) any later version.                                                */
/*                                                                                     */
/*  This program is distributed in the hope that it will be useful,                    */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of                     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                      */
/*  GNU General Public License for more details.                                       */
/*                                                                                     */
/*  You should have received a copy of the GNU General Public License                  */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.              */
/* *********************************************************************************** */
#include "PCF8574.h"

PCF8574 ioModule;

/* *********************************************************************************** */
/* Initialization                                                                      */
/* *********************************************************************************** */
void setup( void ) {
    // initialize I2C bus
    ioModule.init(0x38, D2, D1);
    
    // four output LEDs
    ioModule.pinMode(0, OUTPUT);
    ioModule.pinMode(1, OUTPUT);
    ioModule.pinMode(2, OUTPUT);
    ioModule.pinMode(3, OUTPUT);
    
    // two input buttons
    ioModule.pinMode(4, INPUT);
    ioModule.pinMode(5, INPUT);
    
    // blink all LEDs two times
    ioModule.write8(0);
    for (int i=0; i<2; i++) {
        ioModule.write8(0xff);
        delay(500);
        ioModule.write8(0);
        delay(500);
    }
}

/* *********************************************************************************** */
/* M A I N   L O O P                                                                   */
/* ----------------------------------------------------------------------------------- */
/* Light one LED at a time, loop over all four LEDs                                    */
/* Stop Button halts the alternation, START button continues                           */
/* *********************************************************************************** */
void loop( void ) {
    static int led=0;
    static bool active=true;
    
    // read input buttons
    int btnStart = ioModule.digitalRead(4);
    int btnStop  = ioModule.digitalRead(5);
    
    // shift lit LED
    if ( active ) {
        if ( btnStop == HIGH ) {
            active = false;
        }
        ioModule.digitalWrite(led, LOW);
        led++;
        if ( led >= 4 ) {
            led=0;
        }
        ioModule.digitalWrite(led, HIGH);
    } else if ( btnStart == HIGH ) {
        active = true;
    }
    delay(100);
}
