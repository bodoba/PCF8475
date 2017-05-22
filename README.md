
# PCF8574

Arduino library to interface with the PCF8475 I2C IO extender. The following methods are supported:

```C++

// Default constructor
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
```

