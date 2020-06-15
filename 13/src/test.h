#include <unistd.h>

#define WORKERS_COUNT (4)

void *test_thread_safety_push_worker(void *arg) {
    Queue *queue = ((Shared *) arg)->queue;
    for (int i = 0; i < queue->length / WORKERS_COUNT - 1; ++i) {
        queue_push(queue, 1);
    }
    pthread_exit(NULL);
}

void test_thread_safety_push() {
    Shared *shared = make_shared();
    for (int i = 0; i < QUEUE_LENGTH; ++i) {
        shared->queue->array[i] = 0;
    }

    pthread_t workers[WORKERS_COUNT];
    for (int i = 0; i < WORKERS_COUNT; ++i) {
        workers[i] = make_thread(test_thread_safety_push_worker, shared);
    }
    for (int i = 0; i < WORKERS_COUNT; ++i) {
        join_thread(workers[i]);
    }

    for (int i = 0; i < QUEUE_LENGTH - WORKERS_COUNT; ++i) {
        assert(shared->queue->array[i] == 1);
    }
}

char visited[QUEUE_LENGTH];

void *test_thread_safety_pop_worker(void *arg) {
    Queue *queue = ((Shared *) arg)->queue;
    for (int i = 0; i < queue->length / WORKERS_COUNT - 1; ++i) {
        int index = queue_pop(queue);
        assert(!visited[index]);
        visited[index] = 1;
    }
    pthread_exit(NULL);
}

void test_thread_safety_pop() {
    Shared *shared = make_shared();
    for (int i = 0; i < QUEUE_LENGTH - WORKERS_COUNT; ++i) {
        queue_push(shared->queue, i);
    }

    pthread_t workers[WORKERS_COUNT];
    for (int i = 0; i < WORKERS_COUNT; ++i) {
        workers[i] = make_thread(test_thread_safety_pop_worker, shared);
    }
    for (int i = 0; i < WORKERS_COUNT; ++i) {
        join_thread(workers[i]);
    }
}

void test_thread_safety() {
    test_thread_safety_push();
    test_thread_safety_pop();
}

void *try_pop_from_empty_queue_worker(void *arg) {
    Queue *queue = ((Shared *) arg)->queue;
    assert(queue_pop(queue) == 1);
    pthread_exit(NULL);
}

void try_pop_from_empty_queue() {
    Shared *shared = make_shared();
    pthread_t worker = make_thread(try_pop_from_empty_queue_worker, shared);
    sleep(1);
    queue_push(shared->queue, 1);
    join_thread(worker);
}

void *try_push_to_full_queue_worker(void *arg) {
    Queue *queue = ((Shared *) arg)->queue;
    queue_push(queue, 1);
    pthread_exit(NULL);
}

void try_push_to_full_queue() {
    Shared *shared = make_shared();
    while (!queue_full(shared->queue)) {
        queue_push(shared->queue, 1);
    }
    pthread_t worker = make_thread(try_push_to_full_queue_worker, shared);
    sleep(1);
    queue_pop(shared->queue);
    join_thread(worker);
}

void threads_queue_test() {
    test_thread_safety();
    try_pop_from_empty_queue();
    try_push_to_full_queue();
}
