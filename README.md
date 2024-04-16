# race-conditions

Demonstration of race-conditions. Race condition with shared variable, simultaneous file access, and simultaneous database access is shown.

### bank_fileio.c
Uses pthreads to access the "balance.txt" file meant to hold "bank" balance. Two threads attempt to modify the same file, with the last one to modify overwriting changes of the first.
