/*
 * 
 */
#include <papi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/time.h>

#define NB_RT_ITERATIONS 2000000

int papi_eventset = PAPI_NULL;

/*
 * This function initializes the papi library
 */ 
void init_papi ()
{
	int ret;

	ret = PAPI_library_init(PAPI_VER_CURRENT);
	if (ret != PAPI_VER_CURRENT) {
		fprintf(stderr, "PAPI error: library init error! %s\n", PAPI_strerror(ret));
		exit(1);
	}

	if ((ret = PAPI_multiplex_init()) != PAPI_OK) {
		fprintf(stderr, "PAPI error: cannot initialize multiplexing %s\n", PAPI_strerror(ret));
		exit(2);
	}

	/* Create the event set to measure */
	if ((ret = PAPI_create_eventset(&papi_eventset)) != PAPI_OK) {
		fprintf(stderr, "PAPI error: cannot craete eventset: %s\n", PAPI_strerror(ret));
		exit(3);
	}

	if ((ret = PAPI_assign_eventset_component(papi_eventset, 0)) != PAPI_OK) {
		fprintf(stderr, "PAPI error: cannot assign eventset component: %s\n", PAPI_strerror(ret));
		exit(4);
	}

	if ((ret = PAPI_set_multiplex(papi_eventset)) != PAPI_OK) {
		fprintf(stderr, "PAPI error: cannot set mutiplexing %s\n", PAPI_strerror(ret));
		exit(5);
	}
}

void set_options ()
{
	int ret;

	PAPI_domain_option_t domain_opt;
	domain_opt.def_cidx = 0;
	domain_opt.eventset = papi_eventset;
	domain_opt.domain   = PAPI_DOM_ALL;

	PAPI_granularity_option_t granul_opt;
	granul_opt.def_cidx = 0;
	granul_opt.eventset = papi_eventset;
	granul_opt.granularity = PAPI_GRN_SYS;

	if ((ret = PAPI_set_opt(PAPI_DOMAIN, (PAPI_option_t*) &domain_opt)) != PAPI_OK) {
		fprintf(stderr, "PAPI error: cannot set domain %s\n", PAPI_strerror(ret));
		exit(6);
	}

	if (
}

int main (int argc, char **argv)
{
	int ret;
	pid_t rt_child;


	return EXIT_SUCCESS;
}
