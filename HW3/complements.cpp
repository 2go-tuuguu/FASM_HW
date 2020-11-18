#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <pthread.h>

using namespace std;

//специальная структура для данных потока
typedef struct {
	int dim; 
	int start;
	int count;
	int** matrix;
	int** result;
} pthrData;

int** get_Minor_Matrix(int** mat, int x, int y, int n)
{
	int i = 0, j = 0;

	int** temp = (int**)malloc(n * sizeof(int*));

	for (int i = 0; i < n; i++) {
		temp[i] = (int*)malloc(n * sizeof(int));
	}

	// Итерируем по элементам матрицы
	for (int row = 0; row < n; row++)
	{
		for (int col = 0; col < n; col++)
		{
			if (row != x && col != y)
			{
				temp[i][j++] = mat[row][col];

				if (j == n - 1)
				{
					j = 0;
					i++;
				}
			}
		}
	}

	return temp;
}

int determinant(int** mat, int n)
{
	int D = 0;

	if (n == 1)
		return mat[0][0];

	int sign = 1;

	for (int f = 0; f < n; f++)
	{
		int** temp = get_Minor_Matrix(mat, 0, f, n);

		D += sign * mat[0][f] * determinant(temp, n - 1);

		sign = -sign;

		free(temp);
	}

	return D;
}

int algebraic_Complement(int** mat, int x, int y, int n) 
{
	int** temp = get_Minor_Matrix(mat, x, y, n);
	int res = pow(-1, x + y + 2) * determinant(temp, n - 1);;
	free(temp);
	return res;
}

void PrintMatrix(const char* title, int** mat, int N) {
	printf("------------------\n%s\n------------------\n", title);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%d  ", mat[i][j]);
		}
		printf("\n");
	}
}

void* threadFunc(void* thread_data) {
	//получаем структуру с данными
	pthrData* data = (pthrData*)thread_data;
	int row_index = data->start;
	for (int i = 0; i < data->count; i++) {
		for (int j = 0; j < data->dim; j++) {
			data->result[row_index][j] = algebraic_Complement(data->matrix, row_index, j, data->dim);
		}
		row_index++;
	}
	return NULL;
}

int get_dim()
{
	int dim;
	cout << "Enter dimension length for square matrix: ";
	cin >> dim;
	if (dim < 2)
	{
		cout << "Dimension length must be more than 2. ";
		return get_dim();
	}
	else
		return dim;
}

int get_threads_count()
{
	int threads_count;
	cout << "Enter number of threads: ";
	cin >> threads_count;
	if (threads_count < 1)
	{
		cout << "Number of threads cannot be less than 1. ";
		return get_threads_count();
	}
	else
		return threads_count;
}

int main() {

	srand(101);

	int dim = get_dim();

	int thread_count = get_threads_count();

	int** matrix = (int**)malloc(dim * sizeof(int*));
	int** result = (int**)malloc(dim * sizeof(int*));

	for (int i = 0; i < dim; i++) {
		matrix[i] = (int*)malloc(dim * sizeof(int));
		result[i] = (int*)malloc(dim * sizeof(int));
	}

	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			matrix[i][j] = rand() % 10;
		}
	}

	PrintMatrix("Matrix", matrix, dim);

	pthread_t* threads = (pthread_t*)malloc(thread_count * sizeof(pthread_t));

	pthrData* threadData = (pthrData*)malloc(thread_count * sizeof(pthrData));

	int rows = ceil(dim / thread_count);

	int start = 0;

	//инициализируем структуры потоков
	for (int i = 0; i < thread_count; i++) {
		int count = 0;
		if (i < thread_count - 1) {
			threadData[i].start = start;
			threadData[i].count = rows;
			start += rows;
		}
		else {
			threadData[i].start = start;
			threadData[i].count = dim - start;
		}

		threadData[i].dim = dim;
		threadData[i].matrix = matrix;
		threadData[i].result = result;
		
		//запускаем поток
		pthread_create(&(threads[i]), NULL, threadFunc, &threadData[i]);
	}

	//ожидаем выполнение всех потоков
	for (int i = 0; i < thread_count; i++)
		pthread_join(threads[i], NULL);

	PrintMatrix("Result", result, dim);

	free(threads);
	free(threadData);
	for (int i = 0; i < dim; i++) {
		free(matrix[i]);
		free(result[i]);
	}
	free(matrix);
	free(result);
	return 0;
}
