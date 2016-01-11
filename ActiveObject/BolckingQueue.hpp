#pragma once

#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>

template <typename T, typename Container = std::deque<T>>
class BlockingQueue
{
private:
	mutable std::mutex      d_mutex;
	std::condition_variable d_condition;
	Container	            d_queue;
public:
	void push(T value) 
	{
		std::lock_guard<std::mutex> lock(this->d_mutex);
		d_queue.push_front(value);
		this->d_condition.notify_one();
	}

	T pop() 
	{
		std::unique_lock<std::mutex> lock(this->d_mutex);
		this->d_condition.wait(lock, [this] { return !this->d_queue.empty(); } );

		T rc(this->d_queue.back());
		this->d_queue.pop_back();
		return rc;
	}

	bool tryPop(T& v, std::chrono::milliseconds dur) 
	{
		std::unique_lock<std::mutex> lock(this->d_mutex);

		if (!this->d_condition.wait_for(lock, dur, [this] { return !this->d_queue.empty(); })) 
		{
			return false;
		}

		v = this->d_queue.back();
		this->d_queue.pop_back();
		return true;
	}

	typename Container::size_type size() const
	{
		std::unique_lock<std::mutex> lock(this->d_mutex);
		return d_queue.size();
	}
};
