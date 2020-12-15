#include "HungarianAlgorithm.h"

void HungarianAlgorithm::Execute(int** matrix) 
{
	subtractSmallestValue(matrix);
}

void HungarianAlgorithm::subtractSmallestValue(int** matrix) 
{
	//In each row, subtract the smallest value for each row from each element.
	for (int i = 0; i < N; i++) 
	{
		int minValue = minInRow(matrix, i);
		for (int j = 0; j < M; j++) 
		{
			matrix[i][j] -= minValue;
		}
	}

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			cout << setw(4) << matrix[i][j];
		}

		cout << endl << endl;
	}


	cout << endl << "++++++++++++++++++++++" << endl;

	//In each column, subtract the smallest value for each column from each element.
	for (int j = 0; j < M; j++)
	{
		int minValue = minInColumn(matrix, j);
		for (int i = 0; i < N; i++)
		{
			matrix[i][j] -= minValue;
		}
	}
}

int HungarianAlgorithm::minInRow(int** matrix, int rowNum) 
{
	int minValue = INT_MAX;

	for (int j = 0; j < M; j++) 
	{
		if (matrix[rowNum][j] < minValue) 
		{
			minValue = matrix[rowNum][j];
		}
	}

	return minValue;
}

int HungarianAlgorithm::minInColumn(int** matrix, int columnNum)
{
	int minValue = INT_MAX;

	for (int i = 0; i < N; i++)
	{
		if (matrix[i][columnNum] < minValue)
		{
			minValue = matrix[i][columnNum];
		}
	}

	return minValue;
}
