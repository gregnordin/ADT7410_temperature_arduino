# Mon, 8/5/19

## Equipment

- PyBoard 1.1
- MacBook Pro

## Communicating

From Terminal on MacBook Pro:

    $ ls /dev/tty.*
    /dev/tty.Bluetooth-Incoming-Port /dev/tty.usbmodem14212
    
    $ screen /dev/tty.usbmodem14212
    
Now you are in the PyBoard Micropython REPL.
    
To reboot PyBoard in new screen:

- `Ctrl-d`: performs a soft reboot

or (hard reboot)

- `Ctrl-a` followed by `d` kills `screen`
- Eject PyBoard in Finder
- Push `RST` button on PyBoard

## First simple code

Open `main.py` on the PyBoard and paste the following code and save:

    import time
    
    max_count = 10
    count = 0
    
    while count <=10:
        print(count)
        count += 1
        time.sleep(1.0)  # Delay for 1 second.
        
In the PyBoard REPL in mac os terminal, do a soft reboot (`Ctrl-d`) and the code in `main.py` will start running. Output of the code will be in the REPL window in place of the usual `>>>` prompt. Once the code has finished running the `>>>` prompt will show back up.

## Use pyb.delay, elapsed_millis

`main.py`:

    import pyb
    
    blue = pyb.LED(4)          # LED object
    
    max_count = 10
    count = 0
    start = pyb.millis()
    
    while count <=10:
        elapsed = pyb.elapsed_millis(start)
        blue.toggle()
        print(count, elapsed)
        count += 1
        pyb.delay(500)  # Delay for 500 ms
    
Output:

    >>> CTRL-D
    MPY: sync filesystems
    MPY: soft reboot
    0 0
    1 500
    2 1000
    3 1500
    4 2000
    5 2500
    6 3000
    7 3500
    8 4000
    9 4500
    10 5000
    MicroPython v1.11 on 2019-05-29; PYBv1.1 with STM32F405RG
    Type "help()" for more information.
    >>>

## Figure out how to set temperature to high resolution (16 bit)

See [I2C](http://docs.micropython.org/en/latest/library/machine.I2C.html) documentation and [adafruit_adt7410.py](https://github.com/adafruit/Adafruit_CircuitPython_ADT7410/blob/master/adafruit_adt7410.py). The latter shows how to use `struct.unpack()` to transform the 2 byte `bytes` object returned over I2C from the ADT7410 as the temperature to a `float` temperature in &deg;C according to pg. 12 of the spec sheet.

Note that bitwise operators in python only operate on `int` objects. Therefore the `bytes` object returned from the config register needs converted to an `int` before being OR'd or AND'd, followed by conversion back to a bytes object so it can be written back to the config register. The former is done with `int.from_bytes()` and the latter with `<variable>.to_bytes()`. In each case the 2nd argument must be `'big'` to denote the endianness of the bytes object. In micropython I think this argument is ignored and the maching native bit order is used, but the argument has to be in the function call or an exception is thrown.

`main.py`

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
        print("I2C device {} at address: {}".format(i, hex(device)))
    
    max_count = 10
    count = 0
    start = pyb.millis()
    
    set_low_resolution(0x48)
    
    while count <= max_count:
        elapsed = pyb.elapsed_millis(start)
        if count == int(max_count / 2):
            set_high_resolution(0x48)
        config_0 = i2c.readfrom_mem(0x48, 0x03, 1)
        # temp = i2c.readfrom_mem(0x48, 0x00, 2)
        # temp_0 = struct.unpack('>h', temp)[0] / 128
        blue.toggle()
        print(count, elapsed, config_0, read_temperature_C(0x48))
        count += 1
        pyb.delay(500)  # Delay for 500 ms
    
    blue.off()

Output

    >>> CTRL-D
    MPY: sync filesystems
    MPY: soft reboot
    I2C device 0 at address: 0x48
    I2C device 1 at address: 0x49
    0 1 b'\x00' 23.3125
    1 502 b'\x00' 23.375
    2 1003 b'\x00' 23.3125
    3 1504 b'\x00' 23.3125
    4 2005 b'\x00' 23.3125
    5 2506 b'\x80' 23.33594
    6 3007 b'\x80' 23.33594
    7 3508 b'\x80' 23.375
    8 4009 b'\x80' 23.3125
    9 4510 b'\x80' 23.35938
    10 5011 b'\x80' 23.32813
    MicroPython v1.11 on 2019-05-29; PYBv1.1 with STM32F405RG
    Type "help()" for more information.
    >>>

## Future To-Do

It would be straightforward to re-write [adafruit_adt7410.py](https://github.com/adafruit/Adafruit_CircuitPython_ADT7410/blob/master/adafruit_adt7410.py) to work with micropython.


# Tue, 8/6/19

## Get temperature from all connected ADT7410's

`main.py`

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

Output

    >>> CNTL-D
    MPY: sync filesystems
    MPY: soft reboot
    I2C device 0 at address: 0x48
    I2C device 1 at address: 0x49
    0,0,23.47656,23.40625
    1,502,23.46875,23.39062
    2,1003,23.46094,23.42969
    3,1504,23.49219,23.41406
    4,2005,23.46094,23.42969
    5,2506,23.48438,23.42969
    6,3007,23.48438,23.40625
    7,3508,23.45313,23.39062
    8,4009,23.44531,23.39844
    9,4510,23.46094,23.39844
    10,5011,23.46875,23.36719
    MicroPython v1.11 on 2019-05-29; PYBv1.1 with STM32F405RG
    Type "help()" for more information.
    >>>
    
## Look into how to read and send data with PC python program

### Information & examples

PyBoard code: [Github - micropython_pyboard/150731_try_USB_VCP/pyboard_code.py](https://github.com/gregnordin/micropython_pyboard/blob/master/150731_try_USB_VCP/pyboard_code.py)  
PC REPL test of PyBoard code: [Github - micropython_pyboard/150731_try_USB_VCP/instructions.txt](https://github.com/gregnordin/micropython_pyboard/blob/master/150731_try_USB_VCP/instructions.txt)  
