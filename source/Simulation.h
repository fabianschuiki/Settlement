/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "model/Construction.h"
#include <SFML/System/Thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <queue>
#include <deque>
#include <gc_allocator.h>


class Simulation
{
public:
	Simulation();
	void start();
	void stop();

	double getStepSize();
	double getRealStepSize();

	void scheduleConstruction(const model::Construction &c);

private:
	sf::Thread thread;
	bool terminate;
	void threadFunc();

	double dt, dtr;
	boost::shared_mutex timing_mutex;

	void advance(double dt);

	// Items to be constructed.
	std::queue<model::Construction, std::deque<model::Construction, gc_allocator<model::Construction> > > constructionQueue;
	boost::shared_mutex constructionQueue_mutex;
};