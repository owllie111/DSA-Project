#ifndef WORKLOAD_H
#define WORKLOAD_H

#include <stddef.h>

typedef struct
{
    int task_id;
    int parent_id;
    size_t start_time;
    size_t finish_time;
    size_t idle_count;
    char command[50];
    int priority;
} WorkloadItem;

#endif