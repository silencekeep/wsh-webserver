#pragma once
#include <thread>
#include <vector>
#include <list>
#include <functional>
#include <mutex>
#include <deque>

class threadpool {
public:
	threadpool(int max_thread_count);
	threadpool() : threadpool(10){}
	~threadpool();
	void enqueue(const std::function<void(void*)>& task, void* arg);
	void cancel_all();
	void dequeue(std::pair<std::function<void(void*)>, void*>& task);
private:
	bool isAllRunning;
	std::vector<std::thread> threads;
	std::deque<std::pair<std::function<void(void*)>, void*>> task_queue;
	std::mutex mtx;
	std::condition_variable cv;
	int max_thread_count;
	void thread_in_pool(int num);
};
