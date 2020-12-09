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

	cout << " New task ADDED: Id - " << task.Id << ", Dur: " << task.Duration << " ms, Pr: " << task.Priority << endl;
	task_queue[task.Priority - 1].push(task);
	findMaxPriority();

	_lock.unlock();
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

	dump();

	Task task = task_queue[currentMaxPriority - 1].front();

	_lock.lock();
	task_queue[currentMaxPriority - 1].pop();
	_lock.unlock();

	findMaxPriority();
	
	cout << " Execution STARTED: Id: " << task.Id << ", Dur: " << task.Duration << " ms, Pr: " << task.Priority << endl;


	int duration = task.Duration;
	while (duration != 0)
	{
		if (duration > 5)
		{
			Sleep(5);
			duration -= 5;
			if (currentMaxPriority != 0 && currentMaxPriority < task.Priority) 
			{
				task.Duration = duration;
				_lock.lock();
				cout << " Execution STOPED: Id: " << task.Id << ", Dur left: " << task.Duration << " ms, Pr: " << task.Priority << endl;
				_lock.unlock();
				AddTask(task);
				break;
			}
		}
		else
		{
			Sleep(task.Duration);
			duration = 0;
			_lock.lock();
			cout << " Execution ENDED: Id: " << task.Id << ", Dur: " << task.Duration << " ms, Pr: " << task.Priority << endl;
			_lock.unlock();

		}

	};
	return;
}

void TaskManager::dump() 
{
	_lock.lock();
	cout << endl << " ==========================================" << endl << endl;
	for (int i = 0; i < MIN_PRIORITY; i++)
	{
		vector<queue<Task>> queue = task_queue;
		cout << "  " <<queue[i].size() << " task with priority " << i + 1 << endl;
		while (!queue[i].empty()) 
		{
			Task task = queue[i].front();
			queue[i].pop();
			cout << "\t Id: " << task.Id << ", Dur: " << task.Duration << " ms, Pr: " << task.Priority << endl;
		}
	}
	cout << endl << " ==========================================" << endl << endl;
	_lock.unlock();
}