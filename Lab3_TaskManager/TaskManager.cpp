#include "TaskManager.h"
#include <Windows.h>

void TaskManager::AddTask(Task task) 
{
	task_queue[task.Priority - 1].push(task);
}

Task* TaskManager::MaxPriority() 
{
	for (int i = 0; i < MIN_PRIORITY; i++) 
	{
		if (!task_queue[i].empty()) 
		{
			return &task_queue[i].front();
		}
	}

	return nullptr;
}

void TaskManager::ExecuteTask() 
{
	Task* task = MaxPriority();

	if (task == nullptr) 
	{
		return;
	}

	for (int i = 0; i < task->Duration; i++) 
	{
		Sleep(1);
		//max_priority();
	}
}