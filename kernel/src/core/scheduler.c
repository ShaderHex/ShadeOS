#include "scheduler.h"
#include <stdint.h>

#define MAX_TASKS 8

static TaskFn tasks[MAX_TASKS];
static int task_count = 0;

void scheduler_add(TaskFn fn) {
    if (task_count < MAX_TASKS)
        tasks[task_count++] = fn;
}

void scheduler_run() {
    while (1) {
        for (int i = 0; i < task_count; i++) {
            tasks[i]();
        }
    }
}
