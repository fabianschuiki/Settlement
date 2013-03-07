/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <SFML/System/Thread.hpp>
#include <boost/thread/shared_mutex.hpp>


class Simulation
{
public:
	Simulation();
	void start();
	void stop();

	double getStepSize();
	double getRealStepSize();

private:
	sf::Thread thread;
	bool terminate;
	void threadFunc();

	double dt, dtr;
	boost::shared_mutex timing_mutex;

	void advance(double dt);
};