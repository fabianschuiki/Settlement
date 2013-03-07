/* Copyright © 2013 Fabian Schuiki */
#include "JobQueue.h"
#include "Job.h"

/**
 * @brief Adds the given Job to the queue.
 *
 * The Job is added to the appropriate priority queue and dispatched to one of
 * the workers as quickly as possible.
 */
void JobQueue::add(Job* job)
{
	assert(job != NULL);
	boost::exclusive_lock lock(mutex);

	// Add the job to the appropriate queue.
	bool queueExisted = queues.count(job->priority);
	Queue &q = queues[job->priority];
	q.jobs.push_front(job);

	// If the queue did not exist beforehand, the count is 1 and does not
	// account for higher priority jobs, which need to be added separately.
	if (!queueExisted) {
		accountForHigherPriorityJobs(q, job->priority);
		for (QueueMap::iterator it = queues.begin(); it != queues.end(); it++) {
			if (it->first > job->priority)
				for (int i = 0; i < it->second.jobs.size)
					q.count.post();
		}
	}

	// Increase the count for all queues with priority higher than this.
	for (QueueMap::iterator it = queues.begin(); it != queues.end(); it++) {
		if (it->first >= job->priority)
			it->second.count.post();
	}
}

/**
 * @brief Pops the next Job off the queue.
 *
 * If there is no Job available with at least the given minPriority, the call
 * blocks until one becomes available.
 */
Job* JobQueue::dispatch(int minPriority)
{
	// Get the queue that matches the requested priority.
	Queue &q;
	{
		boost::exclusive_lock lock(mutex);
		bool queueExisted = queues.count(minPriority);
		q = queues[minPriority];

		// If the queue did not exist beforehand, make sure that the count
		// properly reflects all the higher priority jobs.
		if (!queueExisted) {
			for (QueueMap::iterator it = queues.begin(); it != queues.end(); it++) {
				if (it->first > minPriority)
					for (int i = 0; i < it->second.jobs.size())
						q.count.post();
			}
		}
	}

	// Wait for a task of at least minPriority.
	q.count.wait();

	// Decrease the count for all the queues. This will cause the thread to
	// block until a job becomes available.

}