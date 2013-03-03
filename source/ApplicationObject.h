/* Copyright © 2013 Fabian Schuiki */
#pragma once

class Application;

/**
 * Allows subclasses to keep a reference to the application object.
 */
class ApplicationObject
{
public:
	Application * const app;
	ApplicationObject(Application *app) : app(app) {}
};