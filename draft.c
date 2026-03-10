

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

	pthread_create(&coder1, NULL, (void*)coder_routine(), (void *)sim);
	pthread_join(coder1, NULL);
	
}







