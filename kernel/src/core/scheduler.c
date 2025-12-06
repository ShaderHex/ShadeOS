#include "scheduler.h"
#include <stdint.h>

#define MAX_TASKS 8

static Task tasks[MAX_TASKS];
static int task_count = 0;
int should_yield = 1;

void scheduler_add(TaskFn fn) {
    if (task_count < MAX_TASKS)
        tasks[task_count].fn = fn;
        tasks[task_count].enabled = 1;
        task_count++;
}

void scheduler_run() {
    while (1) {
        for (int i = 0; i < task_count; i++) {
            if(tasks[i].enabled) {
                tasks[i].fn();
            }
        }
    }
}
