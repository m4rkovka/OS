#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

const int MAX_THREAD_NUM = 2047;
const int TEMPLATE_SIZE = 3;

typedef struct Matrix {
	double **arr;
} Matrix;

Matrix *MatrixCreate(int m, int n) {
	Matrix *mrx = (Matrix *) malloc(sizeof(Matrix));
	mrx->arr = (double **) malloc(sizeof(double *) * m);
	for (int i = 0; i < m; i++) {
		mrx->arr[i] = (double *) malloc(sizeof(double) * n);
	}
	return mrx;
}

void MatrixDestroy(Matrix *mrx, int m) {
	for (int i = 0; i < m; i++) {
		free(mrx->arr[i]);
		mrx->arr[i] = NULL;
	}
	free(mrx->arr);
	free(mrx);
}

typedef struct IOMatrix {
	Matrix *InMatrix;
	Matrix *OutMatrix;
	int inM;
	int outM;
	int inN;
	int outN;
} IOMatrix;

void ImageInput(Matrix *mrx, double *data, int m, int n) {
	for (int i = 1; i < m - 1; i++) {
		for (int j = 1; j < n - 1; j++) {
			mrx->arr[i][j] = data[(i - 1) * (n - 2) + (j - 1)];
		}
	}
	mrx->arr[0][0] = mrx->arr[1][1];
	mrx->arr[0][n - 1] = mrx->arr[1][n - 2];
	mrx->arr[n - 1][0] = mrx->arr[n - 2][1];
	mrx->arr[n - 1][n - 1] = mrx->arr[n - 2][n - 2];
	for (int i = 1; i < m - 1; i++) {
		mrx->arr[i][0] = mrx->arr[i][1];
		mrx->arr[i][n - 1] = mrx->arr[i][n - 2];
	}
	for (int j = 1; j < n - 1; j++) {
		mrx->arr[0][j] = mrx->arr[1][j];
		mrx->arr[n - 1][j] = mrx->arr[n - 2][j];
	}
}

IOMatrix *IOMatrixCreate(double *data, int m, int n) {
	IOMatrix *tempMatrix = (IOMatrix *) malloc(sizeof(IOMatrix));
	tempMatrix->InMatrix = MatrixCreate(m + 2, n + 2);
	tempMatrix->inM = m + 2;
	tempMatrix->inN = n + 2;
	tempMatrix->OutMatrix = MatrixCreate(m, n);
	tempMatrix->outM = m;
	tempMatrix->outN = n;

	ImageInput(tempMatrix->InMatrix, data, tempMatrix->inM, tempMatrix->inN);

	return tempMatrix;
}

void IOMatrixDestroy(IOMatrix *mrx) {
	MatrixDestroy(mrx->InMatrix, mrx->inM);
	MatrixDestroy(mrx->OutMatrix, mrx->outM);
	mrx->InMatrix = NULL;
	mrx->OutMatrix = NULL;
	free(mrx);
}

void IMatrixPrint(IOMatrix *mrx) {
	for (int i = 1; i < mrx->inM - 1; i++) {
		for (int j = 1; j < mrx->inN - 1; j++) {
			printf("%f ", mrx->InMatrix->arr[i][j]);
		}
		printf("\n");
	}
}

void OMatrixPrint(IOMatrix *mrx) {
	for (int i = 0; i < mrx->outM; i++) {
		for (int j = 0; j < mrx->outN; j++) {
			printf("%f ", mrx->OutMatrix->arr[i][j]);
		}
		printf("\n");
	}
}

void IOMatrixPrint(IOMatrix *mrx) {
	IMatrixPrint(mrx);
	printf("\n");
	OMatrixPrint(mrx);
}

double min(double a, double b) {
	return (a < b) ? a : b;
}

double max(double a, double b) {
	return (a > b) ? a : b;
}

typedef struct Args {
	IOMatrix *mrx;
	int i;
	int j;
} Args;

Args *ArgsCreate(IOMatrix *matrix, int m, int n) {
	Args *arr = (Args *) malloc(sizeof(Args) * m * n);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			arr[i * n + j].mrx = matrix;
			arr[i * n + j].i = i + 1;
			arr[i * n + j].j = j + 1;
		}
	}
	return arr;
}

void ArgsDestroy(Args *arg) {
	free(arg);
}

void *Dilate(void *arg) {
	Args *targ = (Args *) arg;
	double minElem = targ->mrx->InMatrix->arr[targ->i - 1][targ->j - 1];
	for (int cord1 = 0; cord1 < TEMPLATE_SIZE; cord1++) {
		for (int cord2 = 0; cord2 < TEMPLATE_SIZE; cord2++) {
			minElem = min(targ->mrx->InMatrix->arr[targ->i - 1 + cord1][targ->j - 1 + cord2], minElem);
		}
	}
	targ->mrx->OutMatrix->arr[targ->i - 1][targ->j - 1] = minElem;
}

void *Erode(void *arg) {
	Args *targ = (Args *) arg;
	double maxElem = targ->mrx->InMatrix->arr[targ->i - 1][targ->j - 1];
	for (int cord1 = 0; cord1 < TEMPLATE_SIZE; cord1++) {
		for (int cord2 = 0; cord2 < TEMPLATE_SIZE; cord2++) {
			maxElem = max(targ->mrx->InMatrix->arr[targ->i - 1 + cord1][targ->j - 1 + cord2], maxElem);
		}
	}
	targ->mrx->OutMatrix->arr[targ->i - 1][targ->j - 1] = maxElem;
}
int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Input the number of threads. ./name_of_program num_threads\n");
		return 0;
	}

	int numThreads = atoi(argv[1]);
	numThreads = min(max(numThreads, 1), MAX_THREAD_NUM);
	pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * numThreads);
	int m, n;
	printf("Input size of matrix. Num_row Num_column: ");
	scanf("%d %d", &m, &n);
	if (m <= 0 || n <= 0) {
		printf("Invalid size of matrix\n");
		return 0;
	}
	double *startArr = (double *) malloc(sizeof(double) * m * n);
	for (int i = 0; i < m * n; i++) {
		scanf("%lf", &startArr[i]);
	}
	IOMatrix *ioImage = IOMatrixCreate(startArr, m, n);
	Args *argsArr = ArgsCreate(ioImage, m, n);
	int k;
	printf("Input the amount of filtering: ");
	scanf("%d", &k);
	if (k < 0) {
		printf("Invalid amount of filtering\n");
		return 0;
	}
	else if (k == 0) {
		printf("Dilate and Erode:\n");
		IMatrixPrint(ioImage);
		return 0;
	}

	double *helpArr = (double *) malloc(sizeof(double) * m * n);
	for (int cnt = 0; cnt < k; cnt++) {
		int i = 0;
		while (i < m * n) {
			int t = min(m * n - i, numThreads);
			for (int j = 0; j < t; j++) {
				pthread_create(&threads[j], NULL, Dilate, &argsArr[i]);
				i++;
			}
			for (int j = 0; j < t; j++) {
				pthread_join(threads[j], NULL);
			}
		}
		for (int u = 0; u < m; u++) {
			for (int y = 0; y < n; y++) {
				helpArr[u * n + y] = ioImage->OutMatrix->arr[u][y];
			}
		}
		ImageInput(ioImage->InMatrix, helpArr, m + 2, n + 2);
	}

	printf("Dilate:\n");
	OMatrixPrint(ioImage);

	ImageInput(ioImage->InMatrix, startArr, m + 2, n + 2);

	for (int cnt = 0; cnt < k; cnt++) {
		int i = 0;
		while (i < m * n) {
			int t = min(m * n - i, numThreads);
			for (int j = 0; j < t; j++) {
				pthread_create(&threads[j], NULL, Erode, &argsArr[i]);
				i++;
			}
			for (int j = 0; j < t; j++) {
				pthread_join(threads[j], NULL);
			}
		}
		for (int u = 0; u < m; u++) {
			for (int y = 0; y < n; y++) {
				helpArr[u * n + y] = ioImage->OutMatrix->arr[u][y];
			}
		}
		ImageInput(ioImage->InMatrix, helpArr, m + 2, n + 2);
	}

	printf("Erode:\n");
	OMatrixPrint(ioImage);

	return 0;
}