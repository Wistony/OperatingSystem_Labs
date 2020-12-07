#include "TaskManager.h"

void TaskManager::AddTask(Task task) 
{
	task_queue[task.Priority - 1].push(task);
}

