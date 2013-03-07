/* Copyright © 2013 Fabian Schuiki */
#include "Job.h"

/**
 * @brief Initializes a new Job with the given priority.
 *
 * Note that the priority is a constant and cannot be altered, espsecially
 * after the Job has been added to a JobQueue.
 */
Job::Job(int priority) : priority(priority)
{
}