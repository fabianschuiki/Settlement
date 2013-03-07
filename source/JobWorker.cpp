/* Copyright © 2013 Fabian Schuiki */
#include "JobWorker.h"
#include "Job.h"
#include "JobQueue.h"


JobWorker::JobWorker() : thread(&JobWorker::threadFunc, this)
{
	queue = NULL;
	job = NULL;
}

JobWorker::JobWorker(JobQueue* q) : thread(&JobWorker::threadFunc, this)
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
	boost::mutex::scoped_lock lock(mutex);
	return queue;
}

void JobWorker::setQueue(JobQueue* q)
{
	boost::mutex::scoped_lock lock(mutex);
	queue = q;
}


Job* JobWorker::getJob()
{
	boost::mutex::scoped_lock lock(mutex);
	return job;
}

void JobWorker::setJob(Job* j)
{
	boost::mutex::scoped_lock lock(mutex);
	job = j;
}


void JobWorker::threadFunc()
{
	while (true) {
		// Dispatch the next Job from the queue.
		JobQueue* q;
		{
			boost::mutex::scoped_lock lock(mutex);
			q = queue;
		}
		Job* j = q->dispatch();
		setJob(j);

		// Execute the job.
		j->execute();
		setJob(NULL);
		// Todo: Mark timestamp after job completion.
	}
}