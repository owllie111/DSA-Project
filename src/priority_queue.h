#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "workload.h"
#include <stddef.h>
#include <stdbool.h>

typedef enum
{
    PQ_MIN,
    PQ_MAX
} pq_type_t;
typedef bool (*pq_compare_fn)(WorkloadItem *x, WorkloadItem *y);

typedef struct
{
    WorkloadItem **data;
    size_t length;
    size_t capacity;
    pq_compare_fn cmp;
} PriorityQueue;

void pq_init(PriorityQueue *q, size_t capacity, pq_type_t type); // Initialization of the priority queue.
void pq_push(PriorityQueue *q, WorkloadItem *item); // Inserts a new item into the priority queue.
WorkloadItem *pq_peek(const PriorityQueue *q); // Gets the highest-priority item without removing it.
void pq_pop(PriorityQueue *q); // Removes the highest-priority item.
bool pq_empty(const PriorityQueue *q); // Checks if the priority queue is empty.
void pq_destroy(PriorityQueue *q); // Frees the memory used by the priority queue.
void pq_remove_at(PriorityQueue *q, size_t idx); // Removes an item at a specific index.

#endif