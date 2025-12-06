#pragma once

#include "TSC.h"

typedef void (*TaskFn)();

typedef struct {
    TaskFn fn;
    int enabled;
} Task;

void scheduler_add(TaskFn fn);
void scheduler_run();