#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_MAX 100000 

pthread_mutex_t mutex;

unsigned long long threads_cnt = 0;

void *cr_threads_cnt(void *arg) {
	pthread_mutex_lock(&mutex);
	threads_cnt++;
	pthread_mutex_unlock(&mutex);
	sleep(15);
	return NULL;
}

int main() {
	pthread_t threads[THREAD_MAX];
	int error;
	unsigned int i;
	pthread_mutex_init(&mutex, NULL);

	for (i = 0; i < THREAD_MAX; i++) {
		error = pthread_create(&threads[i], NULL, &cr_threads_cnt, NULL);
		if (error != 0) {
			break;
		}
	}

	for (unsigned int j = 0; j < i; j++) {
		pthread_join(threads[j], NULL);
	}

	pthread_mutex_destroy(&mutex);


	printf("Maximum number of threads per process: %u (%llu)\n", i, threads_cnt);
	return 0;
}