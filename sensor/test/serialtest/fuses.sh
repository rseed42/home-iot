#!/bin/bash
# avrdude -p m328 -C avrdude_spi.conf -c myspi -v -P /dev/spidev0.0 -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

avrdude -p m328 -C avrdude_spi.conf -c myspi -v -P /dev/spidev0.0 -U lfuse:w:0xff:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
