#!/bin/sh

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

cd $SCRIPTPATH
# Actual compilation command
cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=cmake/gcc-arm-none-eabi.cmake && cmake --build build
# Convert .elf into .bin for flashing
arm-none-eabi-objcopy -O binary build/drive-swerve.elf build/drive-swerve.bin
