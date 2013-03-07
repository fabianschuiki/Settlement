/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <gc_cpp.h>
#include <boost/interprocess/sync/named_semaphore.hpp>
#include <boost/thread/mutex.hpp>
#include <priority_queue>
#include <vector>

class Job;
class JobWorker;

/**
 * @brief Queue of jobs ready for execution.
 *
 * The JobQueue maintains a priority queue of Job objects that are ready for
 * execution. Typically, multiple JobWorker objects will wait at the queue for
 * a job to become available and will then execute it.
 *
 * Garbage collecting the JobQueue might pose the risk that the mutex and sema-
 * phore are not properly destroyed. Keep an eye out for such issues.
 */

class JobQueue : public gc
{
public:
	void add(Job* job);

protected:
	friend class JobWorker;
	Job* dispatch();

private:
	/**
	 * @brief Number of jobs in the queue.
	 *
	 * Job workers will halt at this semaphore until another process becomes
	 * available.
	 */
	boost::interprocess::named_semaphore count;

	/**
	 * @brief Jobs to be executed.
	 *
	 * The queue is ordered according to Job priority, so that high priority
	 * jobs are dispatched first.
	 */
	std::priority_queue <Job*, std::vector<Job*, gc_allocator<Job*> > > jobs;

	/// Synchronisation for add() and dispatch().
	boost::thread::mutex mutex;
};