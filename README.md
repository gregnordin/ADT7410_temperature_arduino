# Purpose

Compare a variety of microcontrollers by connecting them one or more [Adafruit ADT7410 temperature sensors](https://www.adafruit.com/product/4089) over I2C. Temperature data is then streamed over serial to python code in a jupyter notebook. The notebook reads the temperature data from serial, saves it to a csv file, and does some simple processing and plotting of the data with pandas.

