# Helios Easycontrol Modbus Sensor emulator

as the latest generation of the central ventilation devices have a modbus for connecting multiple clients for sensoring the air (hunity, temperatur, CO2 or VOC) and controling.
To use this feature you have to buy very expensive sensor modules and you need to distribute the modbus through your complete home...
Well, Helios offers a KNX Module, in case you have this kind of home automation in use. But this modole has just a very limited functionality. It can just provide the measured values of the modbus sensors. You cannot give values from other sensors to the ventilation device.

# The solution
using a small PIC processor and an aditional RS485 tranceiver it is possible to emulate all kind of sensors.
The electronic is inteded to be placed as a head / shield for any raspery pi compatible computer.
There the measured values could be set via I2C. The Helios ventilation device will obey these values for controlling the fan speed.

# Details

Used PIC Controler: PIC18F25K42
Pitfalls: 
 * the TXDE signal for handling the RX/TX of RS485 is inverted in silicon A01
 * the CTS cannot be internaly connected to an unimplemented pin as mentioned in the UM. You need to put it to a real pin and connect it to ground.
 * depending of the PI-Like device you will need to use levelshifter for the I2C signals if running the PIC / RS485 tranceiver @ 5V

How it works:
as all three kind of sensors are based on the same PCB wit just different sensors in place, all use almost identical frames for communication.
These frames are prepared in memory for transmission with the current measured values.

- The baudrate is 57600
- FTF sensors have the addresses 31..38
- VOC sensors have the addresses 51..58
- CO2 sensors have the addresses 41..48
- the Temperature is transmitted in float with an offset of 3°C
- the humidity is tranfered as an uint8
- VOC and CO2 is transfered as a uint16

the timer2 of the PIC is used to sync to the modbus. RS485 is transfered via DMA
the measured values must be put in via I2C:

|register| function
|  00    | bitmask for available FTF sensors (0-8)
|  01    | bitmask for available VOC sensors (0-4)
|  02    | bitmask for availabel CO2 sensors (0-4)
|  03    | command byte
| 04 - 11| Humidity values (uint8)
| 12 - 19| temperatur values (uint8) as (t-10) * 10
| 20 - 27| VOC ppm (uint16) MSB first
| 28 - 35| CO2 ppm (uint16) MSB first


If you find this usefull and like to support me as I had the expenses for
the original sensors you might use paypal: e-gadgets@vodafonemail.de

