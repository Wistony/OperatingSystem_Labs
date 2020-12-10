#include <queue>
#include <iostream>
#include <mutex>
#include <vector>
#include <numeric>
using namespace std;

#define MAX_PRIORITY 1
#define MIN_PRIORITY 16
#define TASK_COUNT 100
#define MAX_DURATION 20
#define INTENSITY 1



struct Task 
{
	unsigned int Id;
	int Duration;
	int Priority;

	Task(int id,int dur, int pr) 
	{
		Id = id;
		Duration = dur;
		Priority = pr;
	}
};

struct CompareTaskPriority
{
	bool operator()(Task const& t1, Task const& t2)
	{ 
		return t1.Priority > t2.Priority;
	}
};

class TaskManager 
{
private:
	recursive_mutex _lock;
	vector<queue<Task>> task_queue;
	vector<Task> waitingTime;

	int currentMaxPriority;
	int taskEnded;
	int taskInQueue;

	void findMaxPriority();

public: 
	TaskManager();
	void AddTask(Task);
	void ExecuteTask();
	void dump();
	vector<int> calculateAverageWaitingTimeByPriority();
	int calculateWaitingTime();
	int calculateAverageWaitingTime();
	bool isEnd();
	bool queueIsEmpty();
};