/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <stack>

/**
 * @brief Powerful logging tool.
 * Enables the use of simple logging macros to output extensive information on
 * where the log appeared and at what time.
 */

class Logger
{
public:
	enum Level {
		kLogError = 0,
		kLogWarning,
		kLogImportant,
		kLogInfo,
		kLogDebug
	};
	
	Logger();

	static Logger* current();
	void push();
	void pop();

	enum Level getLevel() const;
	void setLevel(enum Level l);
	
	void log(enum Level level, const char * file, int line, const char * func, const char * fmt, ...);
	
private:
	enum Level level;
	static std::stack <Logger*> stack;
};

/**
 * Convenience macro that calls app->logger.log() with the current file, line
 * and function, and the parameters handed to the macro.
 */
#define LOG(level, ...) ::Logger::current()->log(Logger::level, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
