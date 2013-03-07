/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <SFML/System/Thread.hpp>
#include <boost/thread/mutex.hpp>

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
	boost::mutex mutex;
	JobQueue* queue;
	Job* job;
	sf::Thread thread;
	void threadFunc();
	void setJob(Job* j);
};