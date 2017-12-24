#!/bin/bash
# avrdude -p m328 -C avrdude_gpio.conf -c pi_1 -v -U flash:w:build-pro-ATmega328/serialtest.hex:i
avrdude -p m328 -C avrdude_spi.conf -c myspi -v -P /dev/spidev0.0 -U flash:w:build-pro-ATmega328/serialtest.hex:i
