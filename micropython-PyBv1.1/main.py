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

