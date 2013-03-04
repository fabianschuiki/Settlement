/* Copyright © 2012 Fabian Schuiki */
#include "Error.h"
#include <cerrno>
#include <cstring>

using std::string;


Error::Error(Error *underlying) : underlying(underlying)
{
}

std::string Error::describeAll()
{
	if (underlying)
		return describe() + " " + underlying->describeAll();
	return describe();
}

std::string Error::what()
{
	return prefix() + describeAll();
}


GenericError::GenericError(string message, Error *underlying) : Error(underlying)
{
	this->message = message;
}

string GenericError::describe()
{
	return message;
}


IOError::IOError(string path) : Error()
{
	this->error = strerror(errno);
	this->path = path;
}

string IOError::describe()
{
	string w = error;
	if (!path.empty()) {
		w += ": \"";
		w += path;
		w += "\"";
	}
	w += '.';
	return w;
}
