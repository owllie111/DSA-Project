CC = gcc
CFLAGS = -Wall -Wextra -O2

sched: main.o sched.o priority_queue.o timeline.o
	$(CC) $(CFLAGS) -o $@ main.o sched.o priority_queue.o timeline.o

main.o: main.c sched.h timeline.h workload.h
	$(CC) $(CFLAGS) -c main.c

sched.o: sched.c sched.h priority_queue.h timeline.h workload.h
	$(CC) $(CFLAGS) -c sched.c

priority_queue.o: priority_queue.c priority_queue.h workload.h
	$(CC) $(CFLAGS) -c priority_queue.c

timeline.o: timeline.c timeline.h priority_queue.h
	$(CC) $(CFLAGS) -c timeline.c

clean:
	rm -f sched *.o
