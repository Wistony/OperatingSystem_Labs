#include <iostream>
#include <iomanip>
#include "HungarianAlgorithm.h"

using namespace std;

int** generate_matrix()
{
	int** matrix;
	matrix = new int* [N];
	for (int i = 0; i < N; i++)
	{
		matrix[i] = new int[M];
	}

	srand(time(0));

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			matrix[i][j] = rand() % 20 + 1;
		}
	}

	return matrix;
}

int main() 
{
	int** matrix;

	matrix = generate_matrix();


	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			cout << setw(4) << matrix[i][j];
		}

		cout << endl << endl;
	}

	HungarianAlgorithm::Execute(matrix);

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			cout << setw(4) << matrix[i][j];
		}

		cout << endl << endl;
	}


	return 0;
}
