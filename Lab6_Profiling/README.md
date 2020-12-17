# OperatingSystem - Lab6

##  Профілювання

### Варіант №1 
**Початковий неоптимізований код:**
```
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
```

В даній лабораторній використано ручне профілювання. В наступному коді рахується час виконання кожної функції та загальний час виконання програми. 

```
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
	printf("\n Inside main()\n\n");
	int typeOfShape = 1;


	int start_time;
	int end_time;

	for (int i = 0; i < 0xffffff; i++);
	{
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
	}

	cout << "Total execution time: " << clock() << " ms" << endl;

	return 0;
}
```

**Результати профілювання неоптимізованого коду:**

![Screenshot1](https://github.com/Wistony/OperatingSystem_Labs/blob/master/Lab6_Profiling/img/1.png)

Добре видно, що майже весь час виконання програми забирає на себе функція DrawCircle() або DrawSquare(). Усе тому що дані функції містять в собі функцію Sleep(), видалимо її. Окрім цього в програмі є цикл, який виконує лише 1 ітерацію, тому видалимо його теж.

**Оптимізуємо даний код:** 

```
void new_func1(void);

void DrawCircle(void)
{
	for (int i = 0; i < 10; i++)
	{
		//process of drawing
	}

	return;
}

static void DrawSquare(void)
{
	for (int i = 0; i < 10; i++)
	{
		//process of drawing
	}
	return;
}

int main(void)
{
	printf("\n Inside main()\n\n");
	int typeOfShape = 1;


	int start_time;
	int end_time;

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

	cout << "Total execution time: " << clock() << " ms" << endl;

	return 0;
}
```

**Результати виконання оптимізованого коду:**

![Screenshot1](https://github.com/Wistony/OperatingSystem_Labs/blob/master/Lab6_Profiling/img/2.png)

За допомогою профілювання було оптимізовано даний код. 





