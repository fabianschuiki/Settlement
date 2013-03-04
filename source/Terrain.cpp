/* Copyright © 2013 Fabian Schuiki */
#include "Terrain.h"

Terrain::Terrain(unsigned int w, unsigned int h) : width(w), height(h)
{
	nodes.reserve(w * h);
	cells.reserve(w * h * 2);
}