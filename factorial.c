#include <getopt.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define NUMBER 20

struct FactorialPart{
	unsigned long long start;
	unsigned long long finish;
}factorialParts[NUMBER];

void getFactorialPart(struct FactorialPart *factorialPart);

pthread_t threads[20];

unsigned long long mod = -1;
unsigned long long result = 1;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv) {
	unsigned long long k = -1;
	int pnum = -1;

	while (1) {
        int current_optind = optind ? optind : 1;
		static struct option options[] = {{"k", required_argument, 0, 0},
										  {"pnum", required_argument, 0, 0},
										  {"mod", required_argument, 0, 0},
										  {0, 0, 0, 0}};

		int option_index = 0;
		int c = getopt_long(argc, argv, "f", options, &option_index);

		if (c == -1) break;

		switch (c) {
			case 0:
				switch (option_index) {
						case 0:
							k = atoi(optarg);
							if (k <= 0) {
								printf("k is a positive number\n");
								return 1;
							}
							break;
						case 1:
							pnum = atoi(optarg);
							if (pnum <= 0) {
								printf("pnum is a positive number\n");
								return 1;
							}
							break;
						case 2:
							mod = atoi(optarg);
							if (mod <= 0) {
								printf("mod is a positive number\n");
								return 1;
							}
							break;
						defalut:
							printf("Index %d is out of options\n", option_index);
				}
				break;
			default:
				printf("getopt returned character code 0%o?\n", c);
		}
	}

	if (optind < argc) {
		printf("Has at least one no option argument\n");
		return 1;
	}

	if (k == -1 || pnum == -1 || mod == -1) {
		printf("Usage: %s --k \"num\" --pnum \"num\" --mod \"num\" \n", argv[0]);
		return 1;
	}

	if (pnum > NUMBER){
		printf("pnum > %d\n", NUMBER);
		return 1;
	}

	factorialParts[0].start = 1;
	for (int i = 1; i < pnum; i++){
		int start = i * k / pnum;
		factorialParts[i - 1].finish = start;
		factorialParts[i].start = start + 1;
	}
	factorialParts[pnum - 1].finish = k;

	
	struct timeval start_time;
	gettimeofday(&start_time, NULL);

	for (int i = 0; i < pnum; i++){
		if (pthread_create(&threads[i], NULL, (void *)getFactorialPart,	(void *)&factorialParts[i]) != 0) {
			exit(1);
		}
	}

	for (int i = 0; i < pnum; i++){
		if (pthread_join(threads[i], NULL) != 0) {
			exit(1);
		}
	}

	struct timeval finish_time;
	gettimeofday(&finish_time, NULL);

	double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
	elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

	printf("result = %llu\n", result);
	printf("Elapsed time: %fms\n", elapsed_time);
	

	return 0;
}

void getFactorialPart(struct FactorialPart *factorialPart) {
    int q = 0;
    pthread_mutex_lock(&mut);
	for (unsigned long long i = factorialPart->start; i <= factorialPart->finish; i++){
	    unsigned long long t = (result * i) % mod;
        if (t == 0 && q == 0){
            printf("%10llu * %10llu / %10llu = 0\n", result, i, mod);
            q = 1;
        }

		result = t;
	}
    pthread_mutex_unlock(&mut);
		
	printf("%10llu - %10llu\n", factorialPart->start, factorialPart->finish);
}