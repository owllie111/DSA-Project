#include "timeline.h"
#include "workload.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "sched.h"

bool load_input(FILE *fp);
void run_simulation(size_t steps, Timeline *tl);
extern WorkloadItem *records;
extern size_t rec_count;

#define TOTAL_STEPS 30

int main(int argc, char **argv)
{
    FILE *in = stdin;
    if (argc > 1)
    {
        in = fopen(argv[1], "r");
        if (!in)
        {
            fprintf(stderr, "Cannot open %s: %s\n", argv[1], strerror(errno));
            return 1;
        }
    }

    if (!load_input(in))
        return 1;
    if (in != stdin)
        fclose(in);
    if (!rec_count)
        return 0;

    Timeline *tl = tl_create(rec_count, TOTAL_STEPS - 1);
    run_simulation(TOTAL_STEPS, tl);
    tl_print(tl, rec_count);
    free(records);
    return 0;
}
