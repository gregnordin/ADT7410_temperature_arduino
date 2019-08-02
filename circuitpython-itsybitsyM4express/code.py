import time
import board
import busio
import adafruit_adt7410

i2c_bus = busio.I2C(board.SCL, board.SDA)
adt = adafruit_adt7410.ADT7410(i2c_bus, address=0x48)
adt.high_resolution = True
initial = time.monotonic()  # Time in seconds since power on

while True:
    now = time.monotonic()
    print("{},{}".format(now, adt.temperature))
    time.sleep(1.0)
