#include "threadpool.h"
#include <iostream>

threadpool::threadpool(int max_thread_count) : max_thread_count(max_thread_count) {
	isAllRunning = true;
	for (int i = 0; i < max_thread_count; i++) {
		std::thread thr(&threadpool::thread_in_pool, this, i);
		thr.detach();
		threads.emplace_back(std::move(thr));
	}
}

threadpool::~threadpool()
{
	std::unique_lock<std::mutex> lock(mtx);
	isAllRunning = false;
	cv.notify_all();
	for (std::thread& worker : threads)
		if (worker.joinable()) worker.join();
}

void threadpool::enqueue(const std::function<void(void*)>& task, void* arg)
{
	std::unique_lock<std::mutex> lock(mtx);
	task_queue.emplace_back(std::make_pair(task, arg));
	cv.notify_one();
}

void threadpool::cancel_all()
{
	std::unique_lock<std::mutex> lock(mtx);
	isAllRunning = false;
	cv.notify_all();
	for (std::thread& worker : threads)
		if (worker.joinable()) worker.join();
	isAllRunning = true;
	task_queue.clear();
	for (int i = 0; i < max_thread_count; i++) {
		std::thread thr(&threadpool::thread_in_pool, this, i);
		thr.detach();
		threads.emplace_back(std::move(thr));
	}
}

void threadpool::dequeue(std::pair<std::function<void(void*)>, void*>& task)
{
	std::unique_lock<std::mutex> lock(mtx);
	task = task_queue.front();
	task_queue.pop_front();
	cv.notify_all();
}

void threadpool::thread_in_pool(int num)
{
	while (true) {
		std::unique_lock<std::mutex> uniq_lock(this->mtx,std::defer_lock);
		uniq_lock.lock();
		cv.wait(uniq_lock, 
			[&](){ 
				return !isAllRunning || !task_queue.empty();
			}
		);
		if (!isAllRunning) return;
		auto current_task = task_queue.front();
		task_queue.pop_front();
		uniq_lock.unlock();
		(current_task.first)(current_task.second);
		//std::cout << num << std::endl;
	}
}