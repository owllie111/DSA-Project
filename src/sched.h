#ifndef SCHED_H
#define SCHED_H

#include "timeline.h"
#include <stdio.h>
#include <stdbool.h>
#include "workload.h"

extern WorkloadItem *records;
extern size_t rec_count;

bool load_input(FILE *fp); // Loads workload data from input file.
void run_simulation(size_t steps, Timeline *tl); // Runs the main simulation.

#endif
