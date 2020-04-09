# Report

## 1

Output:

> creation time: 47 microseconds
writing time: 286 microseconds
erasing time: 77 microseconds
closing time: 9 microseconds
removing time: 28 microseconds

## 2

Output:

> directly time: 890820 microseconds
stdio time: 7617 microseconds

Writing with stdio is much faster due to buffering.

## 3

See [mytail.c](https://github.com/8symbols/operating-systems/blob/master/5/src/mytail.c).

## 4

See [mystat.c](https://github.com/8symbols/operating-systems/blob/master/5/src/mystat.c) and [file_status_printer.h](https://github.com/8symbols/operating-systems/blob/master/5/src/file_status_printer.h).

Output:

> ID of containing device:  [8, 17]
File type:                directory
I-node number:            2048382
Mode:                     40755 (octal)
Link count:               4
Ownership:                UID = 1000   GID = 985
Preferred I/O block size: 4096 bytes
File size:                4096 bytes
Blocks allocated:         8
Last status change:       Wed Apr  8 23:13:42 2020
Last file access:         Wed Apr  8 23:13:42 2020
Last file modification:   Wed Apr  8 23:13:42 2020

The hard links count of a directory is 2 + the number of directories immediately contained by it.

## 5

See [myls.c](https://github.com/8symbols/operating-systems/blob/master/5/src/myls.c).

## 6

See [6.c](https://github.com/8symbols/operating-systems/blob/master/5/src/6.c).

~~`--help`~~ `-maxdepth`_`levels`_ was chosen as an extra option.

> Descend at most _levels_ (a non-negative integer) levels of directories below the starting-points. **-maxdepth 0** means only apply the tests and actions to the starting-points themselves.
