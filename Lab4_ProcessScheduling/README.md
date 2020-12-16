# OperatingSystem - Lab4

##  Дослідження принципів проектування та роботи динамічного та статичного плану-вання
### Варіант №2 - Скласти програму реалізації угорського методу для неоднорідної обчислювальної системи.
#
**Угорський алгоритм** — алгоритм комбінаторної оптимізації, що розв'язує задачу про призначення за поліноміальний час. 

**Матрична інтерпретація**

Дано N ресурсів та N завдань, а також матриця NxN, що містить час виконання завдання кожним з ресурсыв. Для того, щоб знайти розподіл обов'язків з мінімальними загальними витратами, необхідно виконати наступні кроки:

1) Впорядкувати інформацію в матриці таким чином, щоб рядки матриці представляли «ресурси», а колонки — «завдання», тоді як кожен елемент матриці представляє витрати на виконання певним ресурсом певного завдання.
2) В кожному рядку від кожного елемента відняти найменше значення для даного рядка.
3) В кожному стовпці від кожного елемента відняти найменше значення для даного стовпця.
4) Знайти максимальне паросполучення. Якщо ми отримаємо рішення, то переходимо на пункт 8 алгоритму). Якщо ні, виконуємо наступний пункт.
5) Від кожного елементу матриці відняти мінімальний елемент.
6) Додати мінімальний з не викреслених елементів до елементів, які знаходяться на перетині викреслених стовпців та рядків.
7) Перейти до пункту 4.
8) Перенести розподіл на початкову матрицю. Цей розподіл покаже який «ресурс» яке «завдання» має виконати, а сума виділених елементів покаже загальну вартість виконання робіт.

**Часова складність** оригінального алгоритму - **O(n^4)**

#

### Приклад роботи програми

Увесь програмний код реалізації алгоритму можна знайти у файлах HungarianAlgrorithm.cpp та HungarianAlgrorithm.h.

Код функції Main:
```
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
	cout << " Total cost of tasks: " << calculateSum(matrix) << endl;

	return 0;
```
	
Програма після виконання виводить наступну інформацію у консоль:

![Screenshot1](https://github.com/Wistony/OperatingSystem_Labs/blob/master/Lab4_ProcessScheduling/img/1.png)

Спочатку виводиться початкова матриця, далі виводиться результуюча матриця із мінімальними призначеннями, після чого виводиться сума призначених елементів, яка показує загальну вартість виконання робіт.