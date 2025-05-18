#ifndef TIMELINE_H
#define TIMELINE_H

#include "priority_queue.h"
#include "workload.h"
#include <stdlib.h>

typedef enum
{
    STATE_INACTIVE,
    STATE_RUNNING,
    STATE_WAITING
} ExecState;

typedef struct
{
    ExecState **grid;
    size_t time_steps;
} Timeline;

Timeline *tl_create(size_t num_tasks, size_t total_steps); // Creates and initializes a timeline grid for tracking task execution states.
void tl_record(size_t t, PriorityQueue *runQ, PriorityQueue *waitQ, Timeline *tl, WorkloadItem *tasks, size_t num_tasks); // Records the state of processes (running, waiting, inactive) at a given timestep based on the current state of runQ and waitQ. 
void tl_print(const Timeline *tl, size_t num_tasks); // Prints the timeline grid showing task states over all time steps.
void tl_free(Timeline *tl); // Frees all memory used by the timeline grid.

#endif