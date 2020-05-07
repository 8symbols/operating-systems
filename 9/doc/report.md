# Report

## 2

Helgrind output:

```console
=2548== Possible data race during read of size 4 at 0x10C084 by thread #1
==2548== Locks held: none
==2548==    at 0x109516: main (main-race.c:15)
==2548==
==2548== This conflicts with a previous write of size 4 by thread #2
==2548== Locks held: none
==2548==    at 0x1094CE: worker (main-race.c:8)
==2548==    by 0x4840876: mythread_wrapper (hg_intercepts.c:389)
==2548==    by 0x48A846E: start_thread (in /usr/lib/libpthread-2.31.so)
==2548==    by 0x49C03D2: clone (in /usr/lib/libc-2.31.so)
==2548==  Address 0x10c084 is 0 bytes inside data symbol "balance"
==2548==
==2548== ----------------------------------------------------------------
==2548==
==2548== Possible data race during write of size 4 at 0x10C084 by thread #1
==2548== Locks held: none
==2548==    at 0x10951F: main (main-race.c:15)
==2548==
==2548== This conflicts with a previous write of size 4 by thread #2
==2548== Locks held: none
==2548==    at 0x1094CE: worker (main-race.c:8)
==2548==    by 0x4840876: mythread_wrapper (hg_intercepts.c:389)
==2548==    by 0x48A846E: start_thread (in /usr/lib/libpthread-2.31.so)
==2548==    by 0x49C03D2: clone (in /usr/lib/libc-2.31.so)
==2548==  Address 0x10c084 is 0 bytes inside data symbol "balance"
```

Helgrind points that there is a possible data race with using `balance` variable.

The fix is to lock each critical section:

```c
Pthread_mutex_lock(&lock);
balance++;
Pthread_mutex_unlock(&lock);
```

## 3

Helgrind output:

```console
==4626== Thread #3: lock order "0x10C0A0 before 0x10C0E0" violated
==4626==
==4626== Observed (incorrect) order is: acquisition of lock at 0x10C0E0
==4626==    at 0x483DC3F: mutex_lock_WRK (hg_intercepts.c:909)
==4626==    by 0x1092C6: Pthread_mutex_lock (mythreads.h:23)
==4626==    by 0x1094F5: worker (main-deadlock.c:13)
==4626==    by 0x4840876: mythread_wrapper (hg_intercepts.c:389)
==4626==    by 0x48A846E: start_thread (in /usr/lib/libpthread-2.31.so)
==4626==    by 0x49C03D2: clone (in /usr/lib/libc-2.31.so)
==4626==
==4626==  followed by a later acquisition of lock at 0x10C0A0
==4626==    at 0x483DC3F: mutex_lock_WRK (hg_intercepts.c:909)
==4626==    by 0x1092C6: Pthread_mutex_lock (mythreads.h:23)
==4626==    by 0x109501: worker (main-deadlock.c:14)
==4626==    by 0x4840876: mythread_wrapper (hg_intercepts.c:389)
==4626==    by 0x48A846E: start_thread (in /usr/lib/libpthread-2.31.so)
==4626==    by 0x49C03D2: clone (in /usr/lib/libc-2.31.so)
==4626==
==4626== Required order was established by acquisition of lock at 0x10C0A0
==4626==    at 0x483DC3F: mutex_lock_WRK (hg_intercepts.c:909)
==4626==    by 0x1092C6: Pthread_mutex_lock (mythreads.h:23)
==4626==    by 0x1094DB: worker (main-deadlock.c:10)
==4626==    by 0x4840876: mythread_wrapper (hg_intercepts.c:389)
==4626==    by 0x48A846E: start_thread (in /usr/lib/libpthread-2.31.so)
==4626==    by 0x49C03D2: clone (in /usr/lib/libc-2.31.so)
==4626==
==4626==  followed by a later acquisition of lock at 0x10C0E0
==4626==    at 0x483DC3F: mutex_lock_WRK (hg_intercepts.c:909)
==4626==    by 0x1092C6: Pthread_mutex_lock (mythreads.h:23)
==4626==    by 0x1094E7: worker (main-deadlock.c:11)
==4626==    by 0x4840876: mythread_wrapper (hg_intercepts.c:389)
==4626==    by 0x48A846E: start_thread (in /usr/lib/libpthread-2.31.so)
==4626==    by 0x49C03D2: clone (in /usr/lib/libc-2.31.so)
==4626==
==4626==  Lock at 0x10C0A0 was first observed
==4626==    at 0x483DC3F: mutex_lock_WRK (hg_intercepts.c:909)
==4626==    by 0x1092C6: Pthread_mutex_lock (mythreads.h:23)
==4626==    by 0x1094DB: worker (main-deadlock.c:10)
==4626==    by 0x4840876: mythread_wrapper (hg_intercepts.c:389)
==4626==    by 0x48A846E: start_thread (in /usr/lib/libpthread-2.31.so)
==4626==    by 0x49C03D2: clone (in /usr/lib/libc-2.31.so)
==4626==  Address 0x10c0a0 is 0 bytes inside data symbol "m1"
==4626==
==4626==  Lock at 0x10C0E0 was first observed
==4626==    at 0x483DC3F: mutex_lock_WRK (hg_intercepts.c:909)
==4626==    by 0x1092C6: Pthread_mutex_lock (mythreads.h:23)
==4626==    by 0x1094E7: worker (main-deadlock.c:11)
==4626==    by 0x4840876: mythread_wrapper (hg_intercepts.c:389)
==4626==    by 0x48A846E: start_thread (in /usr/lib/libpthread-2.31.so)
==4626==    by 0x49C03D2: clone (in /usr/lib/libc-2.31.so)
==4626==  Address 0x10c0e0 is 0 bytes inside data symbol "m2"
==4626==
==4626==
==4626==
==4626== Use --history-level=approx or =none to gain increased speed, at
==4626== the cost of reduced accuracy of conflicting-access information
==4626== For lists of detected and suppressed errors, rerun with: -s
==4626== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 7 from 7)
```

Helgrind points that there is an incorrect lock order.

## 5

Helgrind output:

```console
==4986== Thread #3: lock order "0x10C0E0 before 0x10C120" violated
==4986==
==4986== Observed (incorrect) order is: acquisition of lock at 0x10C120
==4986==    at 0x483DC3F: mutex_lock_WRK (hg_intercepts.c:909)
==4986==    by 0x1092C6: Pthread_mutex_lock (mythreads.h:23)
==4986==    by 0x109501: worker (main-deadlock-global.c:15)
==4986==    by 0x4840876: mythread_wrapper (hg_intercepts.c:389)
==4986==    by 0x48A846E: start_thread (in /usr/lib/libpthread-2.31.so)
==4986==    by 0x49C03D2: clone (in /usr/lib/libc-2.31.so)
==4986==
==4986==  followed by a later acquisition of lock at 0x10C0E0
==4986==    at 0x483DC3F: mutex_lock_WRK (hg_intercepts.c:909)
==4986==    by 0x1092C6: Pthread_mutex_lock (mythreads.h:23)
==4986==    by 0x10950D: worker (main-deadlock-global.c:16)
==4986==    by 0x4840876: mythread_wrapper (hg_intercepts.c:389)
==4986==    by 0x48A846E: start_thread (in /usr/lib/libpthread-2.31.so)
==4986==    by 0x49C03D2: clone (in /usr/lib/libc-2.31.so)
==4986==
==4986== Required order was established by acquisition of lock at 0x10C0E0
==4986==    at 0x483DC3F: mutex_lock_WRK (hg_intercepts.c:909)
==4986==    by 0x1092C6: Pthread_mutex_lock (mythreads.h:23)
==4986==    by 0x1094E7: worker (main-deadlock-global.c:12)
==4986==    by 0x4840876: mythread_wrapper (hg_intercepts.c:389)
==4986==    by 0x48A846E: start_thread (in /usr/lib/libpthread-2.31.so)
==4986==    by 0x49C03D2: clone (in /usr/lib/libc-2.31.so)
==4986==
==4986==  followed by a later acquisition of lock at 0x10C120
==4986==    at 0x483DC3F: mutex_lock_WRK (hg_intercepts.c:909)
==4986==    by 0x1092C6: Pthread_mutex_lock (mythreads.h:23)
==4986==    by 0x1094F3: worker (main-deadlock-global.c:13)
==4986==    by 0x4840876: mythread_wrapper (hg_intercepts.c:389)
==4986==    by 0x48A846E: start_thread (in /usr/lib/libpthread-2.31.so)
==4986==    by 0x49C03D2: clone (in /usr/lib/libc-2.31.so)
==4986==
==4986==  Lock at 0x10C0E0 was first observed
==4986==    at 0x483DC3F: mutex_lock_WRK (hg_intercepts.c:909)
==4986==    by 0x1092C6: Pthread_mutex_lock (mythreads.h:23)
==4986==    by 0x1094E7: worker (main-deadlock-global.c:12)
==4986==    by 0x4840876: mythread_wrapper (hg_intercepts.c:389)
==4986==    by 0x48A846E: start_thread (in /usr/lib/libpthread-2.31.so)
==4986==    by 0x49C03D2: clone (in /usr/lib/libc-2.31.so)
==4986==  Address 0x10c0e0 is 0 bytes inside data symbol "m1"
==4986==
==4986==  Lock at 0x10C120 was first observed
==4986==    at 0x483DC3F: mutex_lock_WRK (hg_intercepts.c:909)
==4986==    by 0x1092C6: Pthread_mutex_lock (mythreads.h:23)
==4986==    by 0x1094F3: worker (main-deadlock-global.c:13)
==4986==    by 0x4840876: mythread_wrapper (hg_intercepts.c:389)
==4986==    by 0x48A846E: start_thread (in /usr/lib/libpthread-2.31.so)
==4986==    by 0x49C03D2: clone (in /usr/lib/libc-2.31.so)
==4986==  Address 0x10c120 is 0 bytes inside data symbol "m2"
==4986==
==4986==
==4986==
==4986== Use --history-level=approx or =none to gain increased speed, at
==4986== the cost of reduced accuracy of conflicting-access information
==4986== For lists of detected and suppressed errors, rerun with: -s
==4986== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 7 from 7)
```

Helgrind indicates the same as in the previous case, although global locking prevents a deadlock.

## 6

Helgrind output:

```console
==5265== Possible data race during read of size 4 at 0x10C08C by thread #1
==5265== Locks held: none
==5265==    at 0x109532: main (main-signal.c:16)
==5265==
==5265== This conflicts with a previous write of size 4 by thread #2
==5265== Locks held: none
==5265==    at 0x1094E5: worker (main-signal.c:9)
==5265==    by 0x4840876: mythread_wrapper (hg_intercepts.c:389)
==5265==    by 0x48A846E: start_thread (in /usr/lib/libpthread-2.31.so)
==5265==    by 0x49C03D2: clone (in /usr/lib/libc-2.31.so)
==5265==  Address 0x10c08c is 0 bytes inside data symbol "done"
==5265==
==5265== ----------------------------------------------------------------
==5265==
==5265== Possible data race during write of size 1 at 0x528B1A5 by thread #1
==5265== Locks held: none
==5265==    at 0x4846526: mempcpy (vg_replace_strmem.c:1537)
==5265==    by 0x4942871: _IO_file_xsputn@@GLIBC_2.2.5 (in /usr/lib/libc-2.31.so)
==5265==    by 0x4938237: puts (in /usr/lib/libc-2.31.so)
==5265==    by 0x109547: main (main-signal.c:18)
==5265==  Address 0x528b1a5 is 21 bytes inside a block of size 1,024 alloc'd
==5265==    at 0x483A7CF: malloc (vg_replace_malloc.c:309)
==5265==    by 0x4935BC3: _IO_file_doallocate (in /usr/lib/libc-2.31.so)
==5265==    by 0x49445FF: _IO_doallocbuf (in /usr/lib/libc-2.31.so)
==5265==    by 0x49437E7: _IO_file_overflow@@GLIBC_2.2.5 (in /usr/lib/libc-2.31.so)
==5265==    by 0x49428F5: _IO_file_xsputn@@GLIBC_2.2.5 (in /usr/lib/libc-2.31.so)
==5265==    by 0x4938237: puts (in /usr/lib/libc-2.31.so)
==5265==    by 0x1094E4: worker (main-signal.c:8)
==5265==    by 0x4840876: mythread_wrapper (hg_intercepts.c:389)
==5265==    by 0x48A846E: start_thread (in /usr/lib/libpthread-2.31.so)
==5265==    by 0x49C03D2: clone (in /usr/lib/libc-2.31.so)
==5265==  Block was alloc'd by thread #2
==5265==
this should print last
==5265==
==5265== Use --history-level=approx or =none to gain increased speed, at
==5265== the cost of reduced accuracy of conflicting-access information
==5265== For lists of detected and suppressed errors, rerun with: -s
==5265== ERROR SUMMARY: 23 errors from 2 contexts (suppressed: 40 from 40)
```

Helgrind points that there is a possible data race with using `balance` variable and `printf()` function.

This is inefficient because spinning of the parent process for a long time just wastes CPU cycles.

## 8

Helgrind output:

```console
==6247== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 7 from 7)
```

This is efficient because `Pthread_cond_wait()` puts the calling thread to sleep, and thus waits for some other thread to signal it.
