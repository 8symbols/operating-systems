# Report

## 1

See [server.c](https://github.com/8symbols/operating-systems/blob/master/8/src/server.c) and [client.c](https://github.com/8symbols/operating-systems/blob/master/8/src/client.c).

The main advantage of async server is that you can run concurrent IO operation using only one thread. The main disadvantage is that it's not always easy to reason about asynchronous operations.
