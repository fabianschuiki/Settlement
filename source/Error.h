/* Copyright © 2012 Fabian Schuiki */
#pragma once
#include <string>
#include <gc_cpp.h>

/**
 * Base class for all errors that may be thrown as C++ exceptions. Defines a
 * common interface to access the error information.
 */
class Error : public gc
{
public:
	Error(Error *underlying = NULL);

	/**
	 * @brief Describes the error.
	 * Only returns the description for this error, not for any underlying
	 * errors if present. Use describeAll() for a recursive description.
	 */
	virtual std::string describe() = 0;

	/**
	 * @brief Returns a prefix for console messages.
	 * The prefix properly introduces the error, e.g. as "Rendering Error: "
	 * or similar.
	 */
	virtual std::string prefix() = 0;

	/**
	 * @brief Recursively describes the error.
	 * Calls itself recursively to generate a concatenated version of all
	 * results of describe() of this and all underlying errors.
	 */
	std::string describeAll();

	/**
	 * @brief Full error message.
	 * Perfectly suited for logging or displaying. The result of this function
	 * contains the error's prefix as well as the entire error chain.
	 */
	std::string what();
protected:
	Error* const underlying;
};

/**
 * A general error that may be described by a simple string.
 */
class GenericError : public Error
{
public:
	GenericError(std::string message, Error *underlying = NULL);
	virtual std::string describe();
	virtual std::string prefix() { return "Error: "; }
protected:
	std::string message;
};

/**
 * An IO error that occurred during interaction with the filesystem. Upon
 * construction, the error looks up the most recent errno and retrieves an
 * appropriate error message for it.
 */
class IOError : public Error
{
public:
	IOError(std::string path = "");
	virtual std::string describe();
	virtual std::string prefix() { return "IO error: "; }
protected:
	std::string path;
	std::string error;
};