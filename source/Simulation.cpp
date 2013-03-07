/* Copyright © 2013 Fabian Schuiki */
#include "Simulation.h"
#include "Logger.h"
#include <SFML/System.hpp>


Simulation::Simulation() : thread(&Simulation::threadFunc, this)
{
	terminate = false;
	dt = 0;
	dtr = 0;
}

void Simulation::start()
{
	terminate = false;
	thread.launch();
}

void Simulation::stop()
{
	terminate = true;
	thread.wait();
}

double Simulation::getStepSize()
{
	boost::shared_lock <boost::shared_mutex> lock(timing_mutex);
	return dt;
}

double Simulation::getRealStepSize()
{
	boost::shared_lock <boost::shared_mutex> lock(timing_mutex);
	return dtr;
}

void Simulation::threadFunc()
{
	sf::Clock clock;
	while (!terminate)
	{
		// Measure the time.
		{
			boost::unique_lock <boost::shared_mutex> lock(timing_mutex);
			dtr = clock.getElapsedTime().asSeconds();
			clock.restart();
			dt = std::min(dtr, 1.0/20);
		}

		// Do the actual simulation work.
		advance(dt);

		// Halt the thread if required to stay below a maximum simulation frequency.
		double waitSeconds = 1.0/60 - clock.getElapsedTime().asSeconds();
		if (waitSeconds > 0) {
			sf::sleep(sf::seconds(waitSeconds));
		}
	}
}

void Simulation::advance(double dt)
{
	
}