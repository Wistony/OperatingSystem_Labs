#include "HungarianAlgorithm.h"

HungarianAlgorithm::HungarianAlgorithm() 
{
	rowsCovered = new bool[N];
	columnsCovered = new bool[N];
	assignment = new int[N];

	sourceMatrix = new int*[N];
	markMatrix = new bool*[N];
	for (int i = 0; i < N; i++) 
	{
		markMatrix[i] = new bool[N];
		sourceMatrix[i] = new int[N];
	}
	resetHelperArrays();
}

void HungarianAlgorithm::Execute(int** matrix)
{
	copySourceMatrix(matrix);
	subtractSmallestValue(matrix);

	while (maxMatching(matrix) != N)
	{
		resetHelperMatrix();
		coverColumnsAndRows(matrix);

		subtractAndAdd(matrix);
		resetHelperArrays();
	}

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (assignment[i] != j) 
			{
				matrix[i][j] = 0;
			}
			else 
			{
				matrix[i][j] = sourceMatrix[i][j];
			}
		}
	}
}

void HungarianAlgorithm::copySourceMatrix(int** matrix) 
{
	for (int i = 0; i < N; i++) 
	{
		for (int j = 0; j < N; j++) 
		{
			sourceMatrix[i][j] = matrix[i][j];
		}
	}
}

void HungarianAlgorithm::subtractSmallestValue(int** matrix)
{
	//In each row, subtract the smallest value for each row from each element.
	for (int i = 0; i < N; i++)
	{
		int minValue = minInRow(matrix, i);
		for (int j = 0; j < N; j++)
		{
			matrix[i][j] -= minValue;
		}
	}

	//In each column, subtract the smallest value for each column from each element.
	for (int j = 0; j < N; j++)
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

	for (int j = 0; j < N; j++)
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


int HungarianAlgorithm::maxMatching(int** matrix)
{
	int maxMatching = 0;
	for (int i = 0; i < N; i++)
	{
		bool* seen = new bool[N];
		for (int i = 0; i < N; i++)
		{
			seen[i] = false;
		}

		if (isMatch(matrix, i, seen))
		{
			maxMatching++;
		}
	}

	return maxMatching;
}

bool HungarianAlgorithm::isMatch(int** matrix, int i, bool* seen)
{
	for (int j = 0; j < N; j++)
	{
		if (matrix[i][j] == 0 && !seen[j])
		{
			seen[j] = true;

			if (assignment[j] < 0 || isMatch(matrix, assignment[j], seen))
			{
				assignment[j] = i;
				return true;
			}
		}
	}
	return false;
}


void HungarianAlgorithm::coverColumnsAndRows(int** matrix) 
{
	for (int i = 0; i < N; i++)
	{
		rowsCovered[i] = true;
	}

	for (int j = 0; j < N; j++)
	{
		if (assignment[j] >= 0)
		{
			markMatrix[assignment[j]][j] = true;
			rowsCovered[assignment[j]] = false;
		}
	}

	bool* visitedRows = new bool[N];
	bool* visitedColumns = new bool[N];
	for (int i = 0; i < N; i++)
	{
		visitedRows[i] = false;
		visitedColumns[i] = false;
		columnsCovered[i] = false;
	}

	bool flag = true;

	while (flag) 
	{
		flag = false;
		for (int i = 0; i < N; i++)
		{
			if (rowsCovered[i] && !visitedRows[i])
			{
				visitedRows[i] = true;
				for (int j = 0; j < N; j++) 
				{
					if (matrix[i][j] == 0) 
					{
						columnsCovered[j] = true;
						flag = true;
					}
				}
			}
		}

		for (int j = 0; j < N; j++) 
		{
			if (columnsCovered[j] && !visitedColumns[j]) 
			{
				visitedColumns[j] = true;
				for (int i = 0; i < N; i++)
				{
					if (markMatrix[i][j])
					{
						rowsCovered[i] = true;
						flag = true;
					}
				}
			}
		}
	}

	for (int i = 0; i < N; i++) 
	{
		rowsCovered[i] = !rowsCovered[i];
	}
}

int HungarianAlgorithm::findMinFromUncovered(int** matrix) 
{
	int minValue = INT_MAX;
	for (int i = 0; i < N; i++) 
	{
		for (int j = 0; j < N; j++) 
		{
			if (!columnsCovered[j] && !rowsCovered[i] && matrix[i][j] < minValue) 
			{
				minValue = matrix[i][j];
			}
		}
	}

	return minValue;
}

void HungarianAlgorithm::subtractAndAdd(int** matrix) 
{
	int minValue = findMinFromUncovered(matrix);

	for (int i = 0; i < N; i++) 
	{
		for (int j = 0; j < N; j++) 
		{
			if (!rowsCovered[i] && !columnsCovered[j]) 
			{
				matrix[i][j] -= minValue;
			}
			if (rowsCovered[i] && columnsCovered[j]) 
			{
				matrix[i][j] += minValue;
			}
		}
	}
}
void HungarianAlgorithm::resetHelperArrays() 
{
	for (int i = 0; i < N; i++)
	{
		rowsCovered[i] = false;
		columnsCovered[i] = false;
		assignment[i] = -1;
	}
}

void HungarianAlgorithm::resetHelperMatrix() 
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			markMatrix[i][j] = false;
		}
	}
}