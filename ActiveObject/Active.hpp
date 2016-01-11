#pragma once

#include <functional>
#include <thread>
#include <memory>

#include "BolckingQueue.hpp"

class Active
{
public:
	typedef std::function<void()> Message;

private:
	bool done;
	std::unique_ptr<BlockingQueue<Message>> queue;
	std::unique_ptr<std::thread> thread;

public:
	Active() : done(false)
	{
		thread = std::unique_ptr<std::thread>(new std::thread([=] {this->Run(); }));
		queue = std::unique_ptr<BlockingQueue<Message>>(new BlockingQueue<Message>());
	};

	~Active()
	{
		Send(([&] {done = true; }));
		thread->join();
	};

	Active(const Active&) = delete;
	Active& operator = (const Active&) = delete;

	void Send(const Message& msg)
	{
		//put message on the queue
		queue->push(msg);
	}

private:

	void Run()
	{
		while (!done)
		{
			Message msg = queue->pop();
			msg();
		}
	}
};