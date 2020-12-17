#include <windows.h>
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;


void new_func1(void);

void DrawCircle(void)
{
	for (int i = 0; i < 10; i++)
	{
		//process of drawing
		Sleep(100);
	}

	return;
}

static void DrawSquare(void)
{
	for (int i = 0; i < 10; i++)
	{
		//process of drawing
		Sleep(100);
	}
	return;
}

int main(void)
{
	printf("\n Inside main()\n");
	int typeOfShape = 1;


	int start_time;
	int end_time;

	switch (typeOfShape % 2 + 1) {
	case 1:
		start_time = clock();
		DrawSquare();
		end_time = clock();
		cout << "DrawSquare execution: " << end_time - start_time << " ms" << endl;
		break;
	case 2:
		start_time = clock();
		DrawCircle();
		end_time = clock();
		cout << "DrawCircle execution: " << end_time - start_time << " ms" << endl;
		break;
	}

	cout << "Execution time: " << clock() << " ms";
	return 0;
}