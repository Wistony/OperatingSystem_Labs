#include <Windows.h>
#include <cstdlib>

#include "TaskManager.h"

TaskManager::TaskManager() 
{
	task_queue = vector<queue<Task>>(32);
	currentMaxPriority = 0;
	taskEnded = 0;
	taskInQueue = 0;
}

void TaskManager::AddTask(Task task) 
{
	_lock.lock();
	cout << " New task ADDED: Id: " << task.Id << ", Dur: " << task.Duration << " ms, Pr: " << task.Priority << endl;
	task_queue[task.Priority - 1].push(task);
	taskInQueue += 1;
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
	taskInQueue -= 1;
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
				Task tsk = task;
				if (waitingTime.empty())
				{
					tsk.Duration = 0;
				}
				else 
				{
					tsk.Duration = waitingTime[waitingTime.size() - 1].Duration + task.Duration - duration;
				}
				waitingTime.push_back(tsk);

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
			Task tsk = task;
			if (waitingTime.empty())
			{
				tsk.Duration = 0;
			}
			else
			{
				tsk.Duration = waitingTime[waitingTime.size() - 1].Duration + task.Duration;
			}
			waitingTime.push_back(tsk);
			duration = 0;
			_lock.lock();
			cout << " Execution ENDED: Id: " << task.Id << ", Dur: " << task.Duration << " ms, Pr: " << task.Priority << endl;
			_lock.unlock();
			taskEnded += 1;
		}

	};
	
	return;
}

vector<int> TaskManager::calculateAverageWaitingTimeByPriority()
{	
	vector<int> averageWaitingTime(MIN_PRIORITY);
	int s = 0;
	int c = 0;
	for (int i = 0; i < MIN_PRIORITY; i++) 
	{
		int sum = 0;
		int count = 0;
		for (int j = 0; j < waitingTime.size(); j++) 
		{
			if (waitingTime[j].Priority == i + 1)
			{
				count += 1;
				sum += waitingTime[j].Duration;
			}
		}
		s += sum;
		c += count;

		if (count != 0) 
		{
			averageWaitingTime[i] = sum / count;
		}
		else 
		{
			averageWaitingTime[i] = 0;
		}

	}

	cout << "GGGGGGGGGGGGGGGGGGGGl: " << s / c << endl;
	cout << "SUM : " << s << endl;
	cout << "WAITING: " << calculateWaitingTime() << endl;

	return averageWaitingTime;
}

int TaskManager::calculateAverageWaitingTime() 
{
	return calculateWaitingTime() / waitingTime.size();
}

int TaskManager::calculateWaitingTime() 
{
	int sum = 0;
	for (int i = 0; i < waitingTime.size(); i++)
	{
		sum += waitingTime[i].Duration;
	}

	return sum;
}

bool TaskManager::isEnd() 
{
	return taskEnded == TASK_COUNT;
}


bool TaskManager::queueIsEmpty() 
{
	return taskInQueue == 0;
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