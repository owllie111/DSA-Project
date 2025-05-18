#include "priority_queue.h"
#include "timeline.h"
#include "workload.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_CAPACITY 20

WorkloadItem *records = NULL;
size_t rec_count = 0, rec_capacity = 0;

// Dynamically grows the records array if it's full.
static void grow_records(void)
{
    if (rec_count < rec_capacity)
        return;
    size_t nc = rec_capacity ? rec_capacity * 2 : 10; // we either double the capacity, or start with 10
    records = realloc(records, nc * sizeof *records);
    if (!records)
    {
        perror("grow_records");
        exit(1);
    }
    rec_capacity = nc;
}

// Loads workload data from input file.
bool load_input(FILE *fp)
{
    while (1)
    {
        WorkloadItem temp = {0};
        int got = fscanf(fp,
                         "%d %d %zu %zu %zu %63s %d",
                         &temp.task_id, &temp.parent_id,
                         &temp.start_time, &temp.finish_time,
                         &temp.idle_count, temp.command,
                         &temp.priority);
        if (got == EOF)
            break;
        grow_records(); // makes sure we have enough space
        records[rec_count++] = temp;
    }
    return true;
}

// Runs the main simulation.
void run_simulation(size_t steps, Timeline *tl)
{
    PriorityQueue runQ, waitQ;
    pq_init(&runQ, rec_count, PQ_MIN); // lower-priority tasks get removed first
    pq_init(&waitQ, rec_count, PQ_MAX); // higher-priority tasks get scheduled first

    int used = 0;
    for (size_t t = 0; t < steps; t++)
    {
        // we take a snapshot of current finish times
        size_t *snap = malloc(rec_count * sizeof *snap);
        for (size_t i = 0; i < rec_count; i++)
            snap[i] = records[i].finish_time;
        // and then remove tasks that have finished
        for (size_t i = 1; i <= runQ.length;)
        {
            WorkloadItem *cur = runQ.data[i];
            if (cur->finish_time < t)
            {
                used -= cur->priority;
                pq_remove_at(&runQ, i);
            }
            else
                ++i;
        }
        // this adds new tasks that start now to wait queue
        for (size_t i = 0; i < rec_count; i++)
            if (records[i].start_time == t)
                pq_push(&waitQ, &records[i]);

        WorkloadItem **stash = malloc(rec_count * sizeof *stash);
        size_t stash_n = 0;
        while (!pq_empty(&waitQ))
        {
            WorkloadItem *cand = pq_peek(&waitQ);
            if (used + cand->priority <= MAX_CAPACITY)
            {
                // if we have enough capacity — we schedule it
                pq_pop(&waitQ);
                pq_push(&runQ, cand);
                used += cand->priority;
                continue;
            }
            // remove lower-priority tasks until within space
            WorkloadItem *low = pq_peek(&runQ);
            if (low && low->priority < cand->priority)
            {
                pq_pop(&waitQ);
                pq_push(&runQ, cand);
                used += cand->priority;
                while (used > MAX_CAPACITY)
                {
                    WorkloadItem *d = pq_peek(&runQ);
                    pq_pop(&runQ);
                    used -= d->priority;
                    if (!d->idle_count)
                        d->finish_time++; // delay if not idle before
                    stash[stash_n++] = d;
                }
            }
            else
            {
                // delay and stash
                pq_pop(&waitQ);
                cand->finish_time++;
                stash[stash_n++] = cand;
            }
        }
        for (size_t i = 0; i < stash_n; i++)
            pq_push(&waitQ, stash[i]);
        free(stash);

        // we update idle tasks that haven't changed finish time
        for (size_t i = 1; i <= waitQ.length; i++)
        {
            WorkloadItem *r = waitQ.data[i];
            if (r->finish_time != snap[r->task_id])
            {
                r->idle_count = 1; // start idling
            }
            else if (r->idle_count)
            {
                r->idle_count++; // continue idling
                r->finish_time++; // end idling
            }
        }
        free(snap);
        // logs current simulation step
        tl_record(t, &runQ, &waitQ, tl, records, rec_count);
    }
    // clean everythimg up
    pq_destroy(&runQ);
    pq_destroy(&waitQ);
}