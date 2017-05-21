/* *********************************************************************************** */
/*                                                                                     */
/*  A class to interface with the PCF8574 IO extender                                  */
/*                                                                                     */
/*  Copyright (c) 2017 by Bodo Bauer <bb@bb-zone.com>                                  */
/*                                                                                     */
/*  Thanks to Rob Tillaart for his initial I2C PCF8574 library code for Arduino        */
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

#ifndef PCF8574_h
#define PCF8574_h

#include <stdio.h>
#include <Wire.h>
#include "Arduino.h"

class PCF8574 {
public:
    PCF8574();
    
    // Initialize I2C interface and reset internal state
    PCF8574(uint8_t address, uint8_t sda_pin=SDA, uint8_t scl_pin=SCL);
    void       init (uint8_t address, uint8_t sda_pin=SDA, uint8_t scl_pin=SCL);
    
    // set/get pin mode to INPUT/OUTPUT
    void       pinMode(uint8_t pin, int mode);
    int        pinMode(uint8_t pin);
    
    // read/write/toggle individual pin
    uint8_t    digitalRead(uint8_t pin);
    void       digitalWrite(uint8_t pin, uint8_t value);
    void       digitalToggle(uint8_t pin);
    
    // read/write all pins at once
    uint8_t    read8();
    void       write8(uint8_t value);
    
    // return error status as int and translate it to text
    int        lastError();
    const char *errorText(int error);
    
    
private:
    uint8_t _address;
    uint8_t _sda;
    uint8_t _scl;
    uint8_t _data;
    uint8_t _input;
    
    uint8_t _mode;
    
    int _error;
    
    char _errorText[6][64] = {
        "success",
        "data too long to fit in transmit buffer",
        "received NACK on transmit of address",
        "received NACK on transmit of data",
        "other error",
        "parameter ouf of range"
    };
};

#endif /* PCF8574_h */
