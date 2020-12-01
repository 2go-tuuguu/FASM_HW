#include <iostream>
#include <cmath>
#include <omp.h>
#include <chrono>

using namespace std;

//Gets minor matrix of a given matrix
int** get_minor_matrix(int** mat, int x, int y, int n)
{
	int i = 0, j = 0;
	int** temp = (int**)malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++) {
		temp[i] = (int*)malloc(n * sizeof(int));
	}

	for (int row = 0; row < n; row++) {
		for (int col = 0; col < n; col++) {
			if (row != x && col != y) {
				temp[i][j++] = mat[row][col];
				if (j == n - 1) {
					j = 0;
					i++;
				}
			}
		}
	}
	return temp;
}

//Calculates the determinant of a matrix
int determinant(int** mat, int n)
{
	int D = 0;
	if (n == 1)
		return mat[0][0];
	int sign = 1;
	for (int f = 0; f < n; f++) {
		int** temp = get_minor_matrix(mat, 0, f, n);
		D += sign * mat[0][f] * determinant(temp, n - 1);
		sign = -sign;
		free(temp);
	}
	return D;
}

//Calculates the algebraic complement of an element of a matrix
int algebraic_Complement(int** mat, int x, int y, int n) 
{
	int** temp = get_minor_matrix(mat, x, y, n);
	int res = pow(-1, x + y + 2) * determinant(temp, n - 1);;
	free(temp);
	return res;
}

//Prints matrix with a title
void print_matrix(const char* title, int** mat, int N) {
	printf("------------------\n%s\n------------------\n", title);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%d  ", mat[i][j]);
		}
		printf("\n");
	}
}

//Gets integer input from user
int get_int_input(string message, string error_message, int lowerBound, int upperBound){
	int input;
	cout << message;
	cin >> input;
	if (input < lowerBound || input > upperBound)
	{
		cout << error_message;
		return get_int_input(message, error_message, lowerBound, upperBound);
	}
	else
		return input;
}

int main() {
	srand(time(NULL));

	int dim = get_int_input("Enter dimension length for square matrix: ", 
						"Dimension length cannot be less than 2 or more than 20.\n", 2, 20);
	int thread_count = get_int_input("Enter number of threads: ", 
						"Number of threads cannot be less than 1 or more than 20.\n", 1, 20);

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

	print_matrix("Matrix", matrix, dim);
    
	//Forks tasks into {thread_count} number of threads
	#pragma omp parallel shared(matrix, result) num_threads(thread_count)
	{
		//Distributes loop into threads
		#pragma omp for
		for (int i = 0; i < dim; i++) {
			for (int j = 0; j < dim; j++) {
            	result[i][j] = algebraic_Complement(matrix, i, j, dim);
			}
		}
		//Critical section
		#pragma omp critical
		{
			cout << "Thread: " << omp_get_thread_num() << " working. " 
			<< "Total threads: " << omp_get_num_threads() << endl;
		}
	}

	print_matrix("Result", result, dim);
	
	for (int i = 0; i < dim; i++) {
		free(matrix[i]);
		free(result[i]);
	}
	free(matrix);
	free(result);
	return 0;
}