#include "timeline.h"
#include <stdio.h>
#include <stdlib.h>

// Creates and initializes a timeline grid for tracking task execution states.
Timeline *tl_create(size_t num_tasks, size_t total_steps)
{
    Timeline *tl = malloc(sizeof *tl); // allocates memory for the timeline structure
    if (!tl)
    {
        perror("tl_create");
        exit(1);
    }
    tl->time_steps = total_steps;
    // we allocate an array of pointers for each task (row)
    tl->grid = calloc(num_tasks, sizeof(ExecState *));
    if (!tl->grid)
    {
        perror("tl_create");
        exit(1);
    }
    // we then allocate each task's row and initialize all entries to STATE_INACTIVE
    for (size_t i = 0; i < num_tasks; i++)
    {
        tl->grid[i] = malloc(total_steps * sizeof(ExecState));
        if (!tl->grid[i])
        {
            perror("tl_create");
            exit(1);
        }
        for (size_t t = 0; t < total_steps; t++)
            tl->grid[i][t] = STATE_INACTIVE; // task is neither waiting nor running
    }
    return tl;
}

// Records the state of processes (running, waiting, inactive) at a given timestep based on the current state of runQ and waitQ.
void tl_record(size_t t, PriorityQueue *runQ, PriorityQueue *waitQ,
               Timeline *tl, WorkloadItem *tasks, size_t num_tasks)
{
    // mark waiting from waitQ
    for (size_t i = 1; i <= waitQ->length; i++)
    {
        WorkloadItem *r = waitQ->data[i];
        if (r->finish_time >= t)
            tl->grid[r->task_id][t] = STATE_WAITING;
    }
    // mark tasks not yet arrived
    for (size_t i = 0; i < num_tasks; i++)
    {
        if (t < tasks[i].start_time)
            tl->grid[i][t] = STATE_WAITING;
    }
    // mark running from runQ
    for (size_t i = 1; i <= runQ->length; i++)
    {
        WorkloadItem *r = runQ->data[i];
        if (r->finish_time >= t)
            tl->grid[r->task_id][t] = STATE_RUNNING;
    }
}

// Prints the timeline grid showing task states over all time steps.
void tl_print(const Timeline *tl, size_t num_tasks)
{
    printf("[===Results===]\n");
    for (size_t i = 0; i < num_tasks; i++)
    {
        printf("%zu\t", i);
        for (size_t t = 0; t < tl->time_steps; t++)
        {
            char c = '_'; // default character for STATE_INACTIVE
            switch (tl->grid[i][t])
            {
            case STATE_WAITING:
                c = '.'; // waiting tasks are shown as .
                break;
            case STATE_RUNNING:
                c = 'R'; // running tasks are shown as R
                break;
            default:
                break;
            }
            putchar(c);
        }
        putchar('\n');
    }
}

// Frees all memory used by the timeline grid.
void tl_free(Timeline *tl)
{
    for (size_t i = 0; i < tl->time_steps; i++)
        free(tl->grid[i]);
    free(tl->grid);
    free(tl);
}