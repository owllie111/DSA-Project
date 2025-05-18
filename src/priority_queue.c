#include "priority_queue.h"
#include <stdlib.h>
#include <stdio.h>

// Swaps two pointers to WorkloadItem
static void pq_swap(WorkloadItem **a, WorkloadItem **b)
{
    WorkloadItem *tmp = *a;
    *a = *b;
    *b = tmp;
}

// Comparison function. Returns true if x has higher priority. (for max-heap)
static bool cmp_greater(WorkloadItem *x, WorkloadItem *y)
{
    if (x->priority == y->priority)
        return x->task_id > y->task_id;
    return x->priority > y->priority;
}


// Comparison function. Returns true if x has lower priority. (for min-heap)
static bool cmp_less(WorkloadItem *x, WorkloadItem *y)
{
    if (x->priority == y->priority)
        return x->task_id > y->task_id;
    return x->priority < y->priority;
}


// Moves an element up the heap to restore heap property.
static void sift_up(PriorityQueue *q, size_t idx)
{
    while (idx > 1)
    {
        size_t parent = idx / 2;
        if (!q->cmp(q->data[idx], q->data[parent]))
            break;
        pq_swap(&q->data[idx], &q->data[parent]);
        idx = parent;
    }
}

// Moves an element down the heap to restore heap property.
static void sift_down(PriorityQueue *q, size_t idx)
{
    for (;;)
    {
        size_t left = idx * 2, right = left + 1, best = idx;
        // check if left child is better
        if (left <= q->length && q->cmp(q->data[left], q->data[best]))
            best = left;
        // check if right child is better
        if (right <= q->length && q->cmp(q->data[right], q->data[best]))
            best = right;
        if (best == idx)
            break;
        pq_swap(&q->data[idx], &q->data[best]);
        idx = best;
    }
}

// Initialization of the priority queue.
void pq_init(PriorityQueue *q, size_t capacity, pq_type_t type)
{
    q->data = malloc((capacity + 1) * sizeof(WorkloadItem *));
    if (!q->data)
    {
        perror("pq_init");
        exit(EXIT_FAILURE);
    }
    q->length = 0;
    q->capacity = capacity;
    q->cmp = (type == PQ_MAX) ? cmp_greater : cmp_less; // we select comparison based on queue type
}

// Inserts a new item into the priority queue.
void pq_push(PriorityQueue *q, WorkloadItem *item)
{
    if (q->length == q->capacity)
    {
        fprintf(stderr, "PriorityQueue full\n");
        return;
    }
    q->data[++q->length] = item;
    sift_up(q, q->length);
}

// Gets the highest-priority item without removing it.
WorkloadItem *pq_peek(const PriorityQueue *q)
{
    return q->length ? q->data[1] : NULL;
}

// Removes the highest-priority item.
void pq_pop(PriorityQueue *q)
{
    if (!q->length)
        return;
    pq_swap(&q->data[1], &q->data[q->length--]);
    if (q->length)
        sift_down(q, 1);
}


// Checks if the priority queue is empty.
bool pq_empty(const PriorityQueue *q)
{
    return q->length == 0;
}

// Frees the memory used by the priority queue.
void pq_destroy(PriorityQueue *q)
{
    free(q->data);
}

// Removes an item at a specific index.
void pq_remove_at(PriorityQueue *q, size_t idx)
{
    if (idx > q->length)
        return;
    q->data[idx] = q->data[q->length--]; // replace with last item
    sift_down(q, idx); // restore heap in both directions
    sift_up(q, idx);
}