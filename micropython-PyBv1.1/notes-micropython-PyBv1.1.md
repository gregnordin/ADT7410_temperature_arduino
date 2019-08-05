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

## Switch to pyb.delay, elapsed_millis

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

