#include <limits>
#include <iostream>
#include <iomanip>

using namespace std;

//number of resourses and number of tasks
#define N 5

class HungarianAlgorithm 
{
private: 
	bool* rowsCovered;
	bool* columnsCovered;
	// assignment[i] is the resourse number for task i
	int* assignment;
	bool** markMatrix;
	int** sourceMatrix;

	void subtractSmallestValue(int**);
	int minInRow(int**, int);
	int minInColumn(int**, int);
	int maxMatching(int**);
	void coverColumnsAndRows(int**);
	bool isMatch(int**, int, bool*);
	int findMinFromUncovered(int**);
	void subtractAndAdd(int**);
	void resetHelperArrays();
	void resetHelperMatrix();
	void copySourceMatrix(int**);

public:
	HungarianAlgorithm();
	void Execute(int**);

};
