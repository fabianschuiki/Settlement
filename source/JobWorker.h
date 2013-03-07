/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Thread.hpp>

class Job;
class JobQueue;

class JobWorker
{
public:
	JobWorker();
	JobWorker(JobQueue* q);

	void start();
	void stop();

	JobQueue* getQueue();
	void setQueue(JobQueue* q);

	Job* getJob();

private:
	sf::Mutex mutex;
	JobQueue* queue;
	Job* job;
	sf::Thread thread;
	void threadFunc();
	void setJob(Job* j);
};