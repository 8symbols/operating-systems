# Report

## 1

See [reader.c](https://github.com/8symbols/operating-systems/blob/master/10/src/reader.c) and [unlinker.c](https://github.com/8symbols/operating-systems/blob/master/10/src/unlinker.c).

Nothing will happen to the read operation by the first process, it will complete successfully.

An excerpt from the `unlink()` [man page](https://www.gnu.org/software/libc/manual/html_node/Deleting-Files.html#Deleting-Files):

>The unlink function deletes the file name *filename*. If this is a file’s sole name, the file itself is also deleted. (**Actually, if any process has the file open when this happens, deletion is postponed until all processes have closed the file.**)

## 2

CPU time = (instructions / program) * (cycles / instruction) * (seconds / cycle) = 2,000,000 * 4 * (1 / 8,000,000) = 1.

## 3

Page number bits = virtual address bits - page offset bits = virtual address bits - log<sub>2</sub>(page size bytes) = 20 - log<sub>2</sub>2<sup>10</sup> = 10.
Number of page entries = 2<sup>page number bits</sup> = 2<sup>10</sup> = 1024.

## 4

Algorithm | Waiting time | Starvation | Turnaround time | Variance in turnaround time
--- | --- | --- | --- | ---
**First Come, First Served (FCFS).** | The average waiting time is not minimal. | Isn't possible. | Depends on the order of processes. | High.
**Shortest Job First (SJF).** | It's optimal with respect to average waiting time if all processes are available at the same time. | Possible. | Increases if a long process starts to run before a short one enters system. | There is no variance for the same set of available processes.
**Shortest Time-to-Completion First (STCF).** | It's optimal. | Possible. | The turnaround time of the long process is improved (relative to SJF). | There is no variance for the same set of processes.
**Round Robin (RR).** | Even the short processes may take long time to execute. | Isn't possible. | Even worse than simple FCFS in many cases. | Depends on the size of the time quantum.

FCFS algorithm has a high variance in turnaround time.
