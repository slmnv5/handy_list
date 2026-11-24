#!/bin/bash

clear
rm *.so *.bin 2>/dev/null

gcc -Wall -Wextra -Wno-unused-function -Wno-unused-variable \
  test.c -o test.bin

./test.bin



