#include <Windows.h>
#include <cstdlib>

#include "TaskManager.h"

TaskManager::TaskManager() 
{
	task_queue = vector<queue<Task>>(32);
	currentMaxPriority = 0;
}

void TaskManager::AddTask(Task task) 
{
	_lock.lock();

	cout << " Generate new task: Id - " << task.Id << ", Duration - " << task.Duration << " ms, Prioriy - " << task.Priority << endl;
	task_queue[task.Priority - 1].push(task);
	findMaxPriority();

	_lock.unlock();

	dump();
}

void TaskManager::findMaxPriority() 
{
	bool haveTask = false;
	for (int i = 0; i < MIN_PRIORITY; i++) 
	{
		if (!task_queue[i].empty()) 
		{
			haveTask = true;
			currentMaxPriority = i + 1;
			break;
		}
	}
	
	if (!haveTask) 
	{
		currentMaxPriority = 0;
	}
}

void TaskManager::ExecuteTask() 
{
	if (currentMaxPriority == 0) 
	{
		return;
	}

	Task task = task_queue[currentMaxPriority - 1].front();

	_lock.lock();
	task_queue[currentMaxPriority - 1].pop();
	_lock.unlock();

	findMaxPriority();
	
	int start = clock();

	int duration = task.Duration;
	while (duration != 0)
	{
		if (duration > 5)
		{
			Sleep(5);
			duration -= 5;
		}
		else {
			Sleep(task.Duration);
			duration= 0;
		}

	};
	//Sleep(task.Duration);
	int end = clock();
	cout << " EXECUTE task: Id - " << task.Id << ", Duration - " << task.Duration << " ms, Priority - " << task.Priority << endl;


	cout << "Time: " << (end - start) << endl;
	return;
}

void TaskManager::dump() 
{
	_lock.lock();
	cout << endl << "======================================================" << endl;
	for (int i = 0; i < MIN_PRIORITY; i++)
	{
		vector<queue<Task>> queue = task_queue;
		cout << queue[i].size() << " task with priority " << i + 1 << endl;
		while (!queue[i].empty()) 
		{
			Task task = queue[i].front();
			queue[i].pop();
			cout << "\t Id: " << task.Id << ", Dur: " << task.Duration << " ms, Pr: " << task.Priority << endl;
		}
	}
	cout << endl << "======================================================" << endl;
	_lock.unlock();
}