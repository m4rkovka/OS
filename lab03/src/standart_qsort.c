#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int MAX;

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

void Quicksort(char **A, int p, int r) {
	if (p < r) {
		int q = Partion(A, p, r);
		Quicksort(A, p, q - 1);
		Quicksort(A, q + 1, r);
	}
}

int main() {
	printf("Input number of elements in the array: ");
	scanf("%d", &MAX);
	char **A = (char **) malloc(sizeof(char *) * MAX);
	for (int i = 0; i < MAX; i++) {
        A[i] = (char*)malloc(sizeof(char)*30);
        scanf("%s", A[i]);
    }
    printf("______________________________________\n");

	Quicksort(A, 0, MAX - 1);
	for (int i = 0; i < MAX; i++) {
		printf("%s\n", A[i]);
	}
	printf("\n");

	return 0;
}