#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
void compile(long time_to_compile);
void debug(long time_to_debug);
void refactor(long time_to_refactor);

typedef struct sim_s {
	int n_coders;
    int time_to_burnout_ms;
    int time_to_compile_ms;
    int time_to_debug_ms;
    int time_to_refactor_ms;
    int number_of_compiles_required;
    int dongle_cooldown_ms;
    int scheduler;
} sim_t;

typedef struct coders_s {
	int id;
	sim_t * sim;
	dongle_t* left_dongle;
	dongle_t* right_dongle;
	int compile_count;
} coders_t;

typedef struct dongle_s{
	pthread_mutex_t mutex;
	int id;
	int used;
	int cooldown_time;
} dongle_t;

void coder_routine(sim_t sim)
{
	
}




int main()
{
	sim_t sim = malloc(sizeof(sim_t));
	
	pthread_t	coder1;
	long time_to_compile = 10000;
	long time_to_debug = 5000;
	long time_to_refactor = 2500;
	long time_to_burnout = 1000000;
	long dongle_cooldown = 2500;
	long number_of_compile = 5;

	pthread_create(&coder1, NULL, (void*)(compile), (void *)time_to_compile);
	pthread_join(coder1, NULL);
	
}

void compile(long time_to_compile)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long ms_now = (long long)tv.tv_sec*1000 + tv.tv_usec/1000;
	long target = ms_now + time_to_compile;
	printf("\nStarted compiling...\n");
	while (ms_now < target)
	{
		gettimeofday(&tv, NULL);
		ms_now = (long long)tv.tv_sec*1000 + tv.tv_usec/1000;
	}
	printf("Compiled!");
}

void debug(long time_to_debug)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long ms = (long long)tv.tv_sec*1000 + tv.tv_usec/1000;
	long target = ms + time_to_debug;
	printf("\nStarted debugging...\n");
	while (ms < target)
	{
		gettimeofday(&tv, NULL);
		ms = (long long)tv.tv_sec*1000 + tv.tv_usec/1000;
	}
	printf("debugd!");
}

void refactor(long time_to_refactor)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long ms = (long long)tv.tv_sec*1000 + tv.tv_usec/1000;
	long target = ms + time_to_refactor;
	printf("\nStarted refactoring...\n");
	while (ms < target)
	{
		gettimeofday(&tv, NULL);
		ms = (long long)tv.tv_sec*1000 + tv.tv_usec/1000;
	}
	printf("refactored!");
}
