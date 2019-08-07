import pyb
from machine import Pin, I2C
import struct

#---------------------------------------------------------
# Function definitions
#
# In future could adapt CircuitPython class at 
# https://github.com/adafruit/Adafruit_CircuitPython_ADT7410/blob/master/adafruit_adt7410.py
#---------------------------------------------------------


def read_temperature_C(address):
    temp = i2c.readfrom_mem(address, 0x00, 2)
    return struct.unpack('>h', temp)[0] / 128

def set_high_resolution(address):
    register = 0x03
    config = i2c.readfrom_mem(address, register, 1)
    # bitwise operators can only operate on int types, not bytes types
    config_int = int.from_bytes(config, 'big')
    new_config = config_int | 0x80
    new_config_bytes = new_config.to_bytes(1, 'big')
    i2c.writeto_mem(address, register, new_config_bytes)

def set_low_resolution(address):
    register = 0x03
    config = i2c.readfrom_mem(address, register, 1)
    # bitwise operators can only operate on int types, not bytes types
    config_int = int.from_bytes(config, 'big')
    new_config = config_int & 0x7F
    new_config_bytes = new_config.to_bytes(1, 'big')
    i2c.writeto_mem(address, register, new_config_bytes)


#---------------------------------------------------------
# Main code
#---------------------------------------------------------

blue = pyb.LED(4)          # LED object

i2c = I2C('X', freq=100000)

devices = i2c.scan()
for i, device in enumerate(devices):
    set_high_resolution(device)
    print("I2C device {} at address: {}".format(i, hex(device)))

max_count = 10
count = 0
start = pyb.millis()

while count <= max_count:
    elapsed = pyb.elapsed_millis(start)
    blue.toggle()
    print(count, elapsed, sep=',', end=',')
    for i, device in enumerate(devices):
        if i == len(devices) - 1:
            end = '\n'
        else:
            end = ','
        print(read_temperature_C(device), end=end)
    count += 1
    pyb.delay(500)  # Delay for 500 ms

blue.off()
