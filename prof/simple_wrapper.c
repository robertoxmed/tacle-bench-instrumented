#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char **argv)
{
	cpu_set_t mask;
	struct sched_param sparam;

	if (argc != 2) {
		fprintf(stderr, "Usage %s <test path>\n", argv[0]);
		exit(-1);
	}

	/* Set priority and affinity mask */
	CPU_ZERO(&mask);
	CPU_SET(3, &mask);

	if (sched_setaffinity(getpid(), sizeof(mask), &mask)) {
		perror("Set affinity error, ");
		exit(1);
	}

	memset(&sparam, 0, sizeof(sparam));
	sparam.sched_priority = sched_get_priority_max(SCHED_FIFO);
	if (sched_setscheduler(getpid(), SCHED_FIFO, &sparam)) {
		perror("Set scheduler error, ");
		exit(2);
	}

	/* Swap to the wanted program */
	if (execl(argv[1], NULL) == -1) {
		perror("execl error, ");
		exit(3);
	}

	return EXIT_SUCCESS;
}
