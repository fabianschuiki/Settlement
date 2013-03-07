/* Copyright © 2013 Fabian Schuiki */
#include "JobQueue.h"
#include "Job.h"

/**
 * @brief Adds the given Job to the queue.
 *
 * The Job is added to the priority queue which automatically reorders itself
 * to account for different priorities.
 */
void JobQueue::add(Job* job)
{
	assert(job != NULL);
	boost::mutex::scoped_lock lock(mutex);
	jobs.push(job);
	count.post();
	// Todo: Mark the timestamp the job was added to the queue.
}

/**
 * @brief Pops the next Job off the queue.
 *
 * If there is no Job, the function blocks the thread until a Job becomes
 * available.
 */
Job* JobQueue::dispatch()
{
	count.wait();
	boost::mutex::scoped_lock lock(mutex);
	Job* job = jobs.top();
	jobs.pop();
	// Todo: Mark the timestamp the job was dispatched.
	return job;
}