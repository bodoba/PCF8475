/* *********************************************************************************** */
/*                                                                                     */
/*  A class to interface with the PCF8574 IO extender                                  */
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

/* *********************************************************************************** */
/* Initialize I2C interface and reset internal state                                   */
/* *********************************************************************************** */
PCF8574::PCF8574(void) {
    PCF8574::init(0);
}

PCF8574::PCF8574(uint8_t address, uint8_t sda_pin, uint8_t scl_pin) {
    PCF8574::init(address, sda_pin, scl_pin);
}

void PCF8574::init(uint8_t address, uint8_t sda_pin, uint8_t scl_pin) {
    _address = address;
    _sda     = sda_pin;
    _scl     = scl_pin;
    _data    = 0;
    _mode    = 0;
    _input   = 0;
    Wire.begin(_sda, _scl);
}

/* *********************************************************************************** */
/* set pin mode to INPUT/OUTPUT                                                        */
/* *********************************************************************************** */
void PCF8574::pinMode(uint8_t pin, int mode) {
    if (pin >=0 && pin <= 7) {
        if (mode==INPUT) {
            // set bit in mode mask
            _mode |= (1<<pin);
        } else if (mode==OUTPUT) {
            // clear bit in mode mask
            _mode &= ~(1<<pin);
        }
    } else {
        _error = 5;
    }
}

/* *********************************************************************************** */
/* get mode of pin INPUT/OUTPUT                                                        */
/* *********************************************************************************** */
int PCF8574::pinMode(uint8_t pin) {
    int pinmode = -1;
    if (pin >=0 && pin <= 7) {
        if ((_mode & (1<<pin)) > 0) {
            pinmode = INPUT;
        } else {
            pinmode = OUTPUT;
        }
    } else {
        _error = 5;
    }
    return pinmode;
}

/* *********************************************************************************** */
/* write to pin                                                                        */
/* *********************************************************************************** */
void PCF8574::digitalWrite(uint8_t pin, uint8_t value) {
    if ( PCF8574::pinMode(pin) == OUTPUT ) {
        if ( value == HIGH ) {
            _data |= (1<<pin);
        } else {
            _data &= ~(1<<pin);
        }
        PCF8574::write8(_data);
    } else {
        _error = 5;
    }
}

/* *********************************************************************************** */
/* toggle output pin status                                                            */
/* *********************************************************************************** */
void PCF8574::digitalToggle(uint8_t pin) {
    if ( PCF8574::pinMode(pin) == OUTPUT ) {
        _data ^= (1<<pin);
        PCF8574::write8(_data);
    } else {
        _error = 5;
    }
}

/* *********************************************************************************** */
/* read status of input pin                                                            */
/* *********************************************************************************** */
uint8_t PCF8574::digitalRead(uint8_t pin) {
    uint8_t status = LOW;
    if ( PCF8574::pinMode(pin) == INPUT ) {
        // refresh input data
        PCF8574::read8();
        
        if ((_input & (1<<pin)) > 0) {
            status = HIGH;
        }
    } else {
        _error = 5;
    }
    return status;
}

/* *********************************************************************************** */
/* write all output pins at once                                                       */
/* *********************************************************************************** */
void PCF8574::write8(uint8_t value) {
    
    // set input pins to high
    value |= _mode;
    
    Wire.beginTransmission(_address);
    _data = value;
    Wire.write(_data);
    _error = Wire.endTransmission();
}

/* *********************************************************************************** */
/* read pin status for all pins                                                        */
/* *********************************************************************************** */
uint8_t PCF8574::read8() {
    
    // write byte with input pins set to high to initialize read
    _data |= _mode;
    
    Wire.beginTransmission(_address);
    Wire.write(_data);
    _error = Wire.endTransmission();
    
    // now read actual state
    if ( !_error ) {
        Wire.beginTransmission(_address);
        Wire.requestFrom(_address, 1);
        _input = Wire.read();
        _error = Wire.endTransmission();
        if ( !_error ) {
            // clear all output pins
            _input &= _mode;
        } else {
            _input = 0;
        }
    }
    return _input;
}

/* *********************************************************************************** */
/* return last error status, 0 indicates success                                       */
/* *********************************************************************************** */
int PCF8574::lastError() {
    int e = _error;
    _error = 0;
    return e;
}

/* *********************************************************************************** */
/* translate error status into descriptive string                                      */
/* *********************************************************************************** */
const char *PCF8574::errorText(int error) {
    
    if ( error >= 0 && error <= 5) {
        return _errorText[error];
    } else {
        return NULL;
    }
}
