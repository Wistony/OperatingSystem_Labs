#include <thread>
#include <chrono>
#include <time.h>
#include <Windows.h>

#include "TaskManager.h"


void AddNewTask(TaskManager &taskManager)
{
	srand(time(0));

	for (int i = 0; i < TASK_COUNT; i++)
	{
		Task task = Task(i + 1, rand() % MAX_DURATION + 1, rand() % MIN_PRIORITY + 1);

		taskManager.AddTask(task);

		Sleep(INTENSITY);
	}

	return;
}


int main() 
{
	TaskManager manager;

	thread thr(AddNewTask,ref(manager));
	thr.detach();

	int downtime = 0;
	while (!manager.isEnd()) 
	{
		int start = clock();
		while (manager.queueIsEmpty()) 
		{
		}
		int end = clock();

		downtime = downtime + end - start;
		manager.ExecuteTask();
	};


	int waitingTime = manager.calculateWaitingTime();
	int avgTime = manager.calculateAverageWaitingTime();
	vector<int> avgTimeByPrior = manager.calculateAverageWaitingTimeByPriority();

	cout << "\n\t*** METRICS ***" << endl;
	cout << " Waiting time: " << waitingTime << " ms" << endl;
	cout << " Downtime: " << downtime << " ms" << endl;
	cout << " Percentage: " << (float)downtime / waitingTime * 100 << "%" << endl;
	cout << " Average waiting time: " << avgTime << " ms" << endl;

	for (int i = 0; i < avgTimeByPrior.size(); i++)
	{
		cout << "  Average waiting time for priority " << i + 1 << ": " << avgTimeByPrior[i] << " ms" << endl;
	}

	return 0;
}