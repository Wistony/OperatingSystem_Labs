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

	for (int i = 0; i < 0xffffff; i++);
	{
		switch (typeOfShape) {
		case 1:
			DrawSquare();
			break;
		case 2:
			DrawCircle();
			break;
		}
	}

	return 0;
}