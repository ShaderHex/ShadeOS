#pragma once

#include "TSC.h"

typedef void (*TaskFn)();

void scheduler_add(TaskFn fn);
void scheduler_run();