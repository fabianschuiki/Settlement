/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <gc_cpp.h>
#include <boost/interprocess/sync/named_semaphore.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <queue>
#include <deque>
#include <map>

class Job;

class JobQueue : public gc
{
public:
	void add(Job* job);

protected:
	Job* dispatch(int minPriority);

private:
	struct Queue {
		/**
		 * Semaphore holding the number of jobs with priority more or equal to
		 * this queue's priority.
		 */
		boost::interprocess::named_semaphore count;

		/**
		 * Queue of jobs of the same priority that need to be executed.
		 */
		typedef std::queue <Job*, std::deque<Job, gc_allocator<Job*> > > Jobs;
		Jobs jobs;

		/// Protects accesses to jobs.
		boost::thread::shared_mutex mutex;
	};

	/**
	 * Map of priority-queue pairs.
	 */
	typedef std::map <int, Queue> QueueMap;
	QueueMap queues;
};