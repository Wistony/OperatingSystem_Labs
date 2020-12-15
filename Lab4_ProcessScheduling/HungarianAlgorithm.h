#include <limits>
#include <iostream>
#include <iomanip>

using namespace std;

//number of resourses
#define N 3
// number of tasks
#define M 3

static class HungarianAlgorithm 
{
private: 
	static void subtractSmallestValue(int**);
	static int minInRow(int**, int);
	static int minInColumn(int**, int);

public:
	static void Execute(int**);

};
