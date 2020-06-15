# Report

## 1

See [test.h](https://github.com/8symbols/operating-systems/blob/master/13/src/test.h).

We test possible gotchas with threads: thread safety, pop from empty queue and push to full queue. The last two are trivial. Thread safety is tested by pushing/popping values to/from queue from multiply threads. Since the threads work in parallel, it's possible that we increment `index` in one thread before performing action on `array[index]` in another thread. In the test, we verify that this doesn't happen.

There is a table with the `test.h` results.

|| `queue.c` | `queue_mutex.c` | `queue_cond.c`
--- | --- | --- | ---
**Thread safety** | ❌ | ✔️ | ✔️
**Pop from empty queue** | ❌ | ❌ | ✔️
**Push to full queue** | ❌ | ❌ | ✔️
