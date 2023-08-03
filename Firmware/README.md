# README.TXT
This is the firmware source code for the Synthesis Technology E951 Looping
Expander. It is coded in C targeting the STM32F030R8 microcontroller and uses
an industry-standard 128x64 OLED with an I2C control interface.

## Building
This project is built with ARM GCC for compiling, OpenOCD with an ST-Link interface
for download and debug and Gnu Make to tie it together. The project is based on
the older ST Standard Peripheral driver library which is included in this
repository. Once all the prerequisites are in place just run
'''
make
'''

## Flashing
1) Ensure openocd isn't running as a server already.

2) Make sure that your ST-Link is connected to the build host via USB and the
the SWD cable is attached to the target hardware

3) run
'''
make flash
'''
4) Done!

## Debugging
1) in a separate window start up openocd as a GDB server
'''
openocd -f openocd.cfg
'''

2) run gdb
'''
arm-none-eabi-gdb main.elf
'''

3) connect to the server within gdb
'''
target extended-remote localhost:3333
'''

4) Start debugging!
