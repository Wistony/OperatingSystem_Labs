#include <iostream>

using namespace std;

void source_code() 
{
    int a[] = { 0,0 };
    int res = 0;

    for (int j = 0; j < 10000000; j++)
    {
        a[0]++;
        a[0]++;
    }

    a[1] = a[0];
}

void updated_code_1()
{
    int a[] = { 0,0 };
    int res = 0;

    for (int j = 0; j < 10000000; j++)
    {
        a[0] += 2;
    }

    a[1] = a[0];
}

void updated_code_2()
{
    int a[] = { 0,0 };
    int res = 0;

    for (int j = 0; j < 10000000; j++)
    {
        res += 2;
    }

    a[1] = a[0] = res;
}

int main()
{
    int N = 2000;
    int executionTime = 0;
    for (int i = 0; i < N; i++)
    {
        int start_time = clock();
        source_code();
        int end_time = clock();
        executionTime = executionTime + end_time - start_time;
    }
    cout << "\tAverage execution time for source_code(): " << executionTime / N << " ms" << endl;

    executionTime = 0;
    for (int i = 0; i < N; i++)
    {
        int start_time = clock();
        updated_code_1();
        int end_time = clock();
        executionTime = executionTime + end_time - start_time;
    }
    cout << "\tAverage execution time for updated_code_1(): " << executionTime / N << " ms" << endl;

    executionTime = 0;
    for (int i = 0; i < N; i++)
    {
        int start_time = clock();
        updated_code_2();
        int end_time = clock();
        executionTime = executionTime + end_time - start_time;
    }
    cout << "\tAverage execution time for updated_code_2(): " << executionTime / N << " ms" << endl;


    return 0;
}

