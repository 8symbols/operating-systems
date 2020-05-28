# Report

## 1

```console
./vector-deadlock -n 2 -l 1 -v
```

It prints before and after each of the threads calls `vector_add()`. The print order changes from run to run.

```console
./vector-deadlock -n 2 -l 800 -v -d
```

A deadlock occurs sometimes.

```console
./vector-deadlock -n 1 -l 800 -v -d
```

A deadlock may occur with any number of threads except one.

## 2

The code avoids deadlock due to the use of locks addresses as a way of ordering lock acquisition.  
When the source and destination vectors are the same we need acquire this vector's lock only one time, otherwise there will be a deadlock.

```console
./vector-global-order -t -n 2 -l 100000 -d
```

The total time is directly proportional to the number of loops and to the number of threads.

```console
./vector-global-order -t -n 2 -l 100000 -d -p
```

Performance improves when the parallelism flag `-p` is turned on.

## 3

The first call to `pthread_mutex_trylock()` is really needed in order to lock the destination vector.

The table shows the dependence of the number of retries on the number of threads with 1,000,000 loops.

Threads | Retries
--- | ---
1 | 0
2 | 114,347
3 | 243,452
4 | 264,924
5 | 712,379
6 | 1,424,799
7 | 1,665,178
8 | 2,330,119

## 4

The global lock is used to avoid deadlocks. The main problem is that the global lock is used even when the vectors being manipulated by each thread is different.

Performance improves when the parallelism flag `-p` is turned on.

## 5

`vector-nolock.c` is atomic only for adding a pair of values and not for the adding vectors as a whole.
It runs slower than `vector-global-order` both with and without the parallelism flag `-p`.
