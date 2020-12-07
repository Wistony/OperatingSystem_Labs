#include <queue>
#include <iostream>
using namespace std;

#define MIN_PRIORITY 1
#define MAX_PRIORITY 32


struct Task 
{
	int Duration;
	int Priority;

	Task(int dur, int pr) 
	{
		Duration = dur;
		Priority = pr;
	}
};

struct CompareTaskPriority
{
	bool operator()(Task const& t1, Task const& t2)
	{ 
		return t1.Priority < t2.Priority;
	}
};

class TaskManager 
{
private:
	vector<queue<Task>> task_queue;

public: 
	void AddTask(Task);
};