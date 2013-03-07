/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <gc_cpp.h>

class Job : public gc
{
public:
	const int priority;
	Job(int priority);
};