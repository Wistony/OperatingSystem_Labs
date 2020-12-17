#include <windows.h>
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

void new_func1(void);

void DrawCircle(void)
{
	for (int i = 0; i < 100; i++)
	{
		//process of drawing
		//Sleep(100);
	}

	return;
}

static void DrawSquare(void)
{
	for (int i = 0; i < 100; i++)
	{
		//process of drawing
		//Sleep(100);
	}
	return;
}

int main(void)
{
	printf("\n Inside main()\n\n");
	int typeOfShape = 1;


	int start_time;
	int end_time;

	//for (int i = 0; i < 0xffffff; i++);
	//{
		switch (typeOfShape) {
		case 1:
			start_time = clock();
			DrawSquare();
			end_time = clock();
			cout << "Execution time DrawSquare(): " << end_time - start_time << " ms" << endl;
			break;
		case 2:
			start_time = clock();
			DrawCircle();
			end_time = clock();
			cout << "Execution time DrawCircle(): " << end_time - start_time << " ms" << endl;
			break;
		}
	//}

	cout << "Total execution time: " << clock() << " ms" << endl;

	return 0;
}