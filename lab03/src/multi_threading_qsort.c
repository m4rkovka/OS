#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int MAX;

int THREAD_MAX;

struct Args {
	char **A;
	int p;
	int r;
};
typedef struct Args Args; 

void swap(char **o1, char **o2) {
	char *tmp = *o1;
	*o1 = *o2;
	*o2 = tmp;
}

int Partion(char **A, int p, int r) {
	char *x = A[r];
	int i = p - 1;
	for (int j = p; j <= r - 1; j++) {
		if (strcmp(A[j], x) <= 0) {
			i++;
			swap(&A[i], &A[j]);
		}
	}
	swap(&A[i + 1], &A[r]);

	return i + 1;
}

void *Quick_sort(void *arg) {
	Args tmp_args = *((Args *) arg);
	
	if (tmp_args.p < tmp_args.r) {
		int q = Partion(tmp_args.A, tmp_args.p, tmp_args.r);
		Args new_left_args;
		new_left_args.A = tmp_args.A;
		new_left_args.p = tmp_args.p;
		new_left_args.r = q - 1;
		if (THREAD_MAX != 0) {
			pthread_t left_thread;
			THREAD_MAX--;
			pthread_create(&left_thread, NULL, &Quick_sort, &new_left_args);
			Args new_right_args;
			new_right_args.A = tmp_args.A;
			new_right_args.p = q + 1;
			new_right_args.r = tmp_args.r;
			Quick_sort(&new_right_args);
			pthread_join(left_thread, NULL);
		}
		else {
			Quick_sort(&new_left_args);
			Args new_right_args;
			new_right_args.A = tmp_args.A;
			new_right_args.p = q + 1;
			new_right_args.r = tmp_args.r;
			Quick_sort(&new_right_args);

		}
	}

	return NULL;
}

int main() {
	printf("Input number of elements in the array: ");
	scanf("%d", &MAX);
	printf("Input THREAD_MAX: ");
	scanf("%d", &THREAD_MAX);
	char **A = (char **) malloc(sizeof(char *) * MAX);
	for (int i = 0; i < MAX; i++) {
        A[i] = (char*)malloc(sizeof(char)*30);
        scanf("%s", A[i]);
    }
	Args begin_args;
	begin_args.A = A;
	begin_args.p = 0;
	begin_args.r = MAX - 1;

	pthread_t begin_thread;
	pthread_create(&begin_thread, NULL, &Quick_sort, &begin_args);

	pthread_join(begin_thread, NULL);
	for (int i = 0; i < MAX; i++) {
		printf("%s\n", A[i]);
	}
	printf("\n");

	return 0;
}