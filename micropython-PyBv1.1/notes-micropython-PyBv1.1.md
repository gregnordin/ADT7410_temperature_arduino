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

