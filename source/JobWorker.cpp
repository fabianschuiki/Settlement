/* Copyright © 2013 Fabian Schuiki */
#include "JobWorker.h"
#include "Job.h"
#include "JobQueue.h"


JobWorker::JobWorker()
{
	queue = NULL;
	job = NULL;
}

JobWorker::JobWorker(JobQueue* q)
{
	queue = q;
	job = NULL;
}


void JobWorker::start()
{
	thread.launch();
}

void JobWorker::stop()
{
	thread.terminate();
}


JobQueue* JobWorker::getQueue()
{
	sf::Lock lock(mutex);
	return queue;
}

void JobWorker::setQueue(JobQueue* q)
{
	sf::Lock lock(mutex);
	queue = q;
}


Job* JobWorker::getJob()
{
	sf::Lock lock(mutex);
	return job;
}

void JobWorker::setJob(Job* j)
{
	sf::Lock lock(mutex);
	job = j;
}


void JobWorker::threadFunc()
{
	
}