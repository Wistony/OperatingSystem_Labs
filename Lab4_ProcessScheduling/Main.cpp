#include <iostream>
#include <iomanip>
#include "HungarianAlgorithm.h"

using namespace std;

int** generateRandomMatrix()
{
	int** matrix;
	matrix = new int* [N];
	for (int i = 0; i < N; i++)
	{
		matrix[i] = new int[N];
	}

	srand(time(0));

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			matrix[i][j] = rand() % 50 + 1;
		}
	}

	return matrix;
}

void printMatrix(int** matrix)
{
	for (int i = 0; i < N; i++)
	{
		cout << "\t\t";
		for (int j = 0; j < N; j++)
		{
			cout <<setw(4) << matrix[i][j];
		}

		cout << endl << endl;
	}
}

int main() 
{
	int** matrix;

	matrix = new int* [N];
	for (int i = 0; i < N; i++) 
	{
		matrix[i] = new int[N];
	}

	matrix = generateRandomMatrix();

	cout << " Initial matrix: " << endl;
	printMatrix(matrix);

	HungarianAlgorithm alg;
	alg.Execute(matrix);

	cout << " Resulting matrix: " << endl;
	printMatrix(matrix);

	return 0;
}
