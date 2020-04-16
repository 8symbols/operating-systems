# Report

## 1

See [check-xor.c](https://github.com/8symbols/operating-systems/blob/master/6/src/check-xor.c).

## 2

See [check-fletcher.c](https://github.com/8symbols/operating-systems/blob/master/6/src/check-fletcher.c).

## 3

See [crc.c](https://github.com/8symbols/operating-systems/blob/master/6/src/crc.c).

## 4

Time is indicated in microseconds.

Size | Xor | Fletcher | Crc
:---: | :---: | :---: | :---:
512 B | 256 | 348 | 260
32 KiB | 663 | 1026 | 883
2 MiB | 11940 | 29050 | 19866

CRC-16 is preferable in terms of time (mostly due to implementation with memoization) and quality of verification.

## 5

See [create-csum.c](https://github.com/8symbols/operating-systems/blob/master/6/src/create-csum.c) and [check-csum.c](https://github.com/8symbols/operating-systems/blob/master/6/src/check-csum.c). CRC-8 is used.
