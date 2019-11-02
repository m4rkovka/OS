#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int MAX;

void swap(char **o1, char **o2) {
	char *tmp = *o1;
	*o1 = *o2;
	*o2 = tmp;
}

int min_int(int i1, int i2) {
	return i1 <= i2 ? i1 : i2;
}

int less_eq_str(char *str1, char *str2) {
	int min, min_str;
	min = min_int(strlen(str1), strlen(str2));
	if (strlen(str1) <= strlen(str2)) {
		min_str = 1;
	}
	else {
		min_str = 0;
	}
	for (int i = 0; i < min; i++) {
		if (str1[i] < str2[i]) {
			return 1;
		} 
		else if (str1[i] > str2[i]) {
			return 0;
		}
	}

	return min_str;
}

int Partion(char **A, int p, int r) {
	char *x = A[r];
	int i = p - 1;
	for (int j = p; j <= r - 1; j++) {
		if (/*less_eq_str(A[j], x)*/strcmp(A[j], x) <= 0) {
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

	Quicksort(A, 0, MAX - 1);
	for (int i = 0; i < MAX; i++) {
		printf("%s\n", A[i]);
	}
	printf("\n");

	return 0;
}