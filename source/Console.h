/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <string>
#include <vector>
#include <gc_cpp.h>
#include <gc_allocator.h>
#include <boost/function.hpp>


typedef std::vector <std::string> ConsoleArgs;

class AbstractConsoleCommand : public gc
{
public:
	std::string name;
	std::string description;

	typedef ConsoleArgs Args;
	virtual bool execute(const Args& args) = 0;

	virtual bool isGroup() { return false; }
};

class ConsoleCommandGroup : public AbstractConsoleCommand
{
public:
	typedef std::vector <AbstractConsoleCommand*, gc_allocator<AbstractConsoleCommand*> > Commands;
	Commands commands;
	
	inline void add(AbstractConsoleCommand* cmd)
	{
		commands.push_back(cmd);
	}
	
	inline void add(ConsoleCommandGroup grp)
	{
		ConsoleCommandGroup* ptr = new ConsoleCommandGroup;
		*ptr = grp;
		add(ptr);
	}

	bool execute(const Args& args)
	{
		if (args.empty())
			return false;
		Args::const_iterator a0 = args.begin();
		for (Commands::iterator c = commands.begin(); c != commands.end(); c++) {
			if ((*c)->name.empty()) {
				if ((*c)->execute(args))
					return true;
			}
			if ((*c)->name == *a0) {
				a0++;
				return (*c)->execute(Args(a0, args.end()));
			}
		}
		return false;
	}

	std::string getHelpPage()
	{
		std::string result;
		if (!name.empty()) {
			result += "Command \"";
			result += name;
			result += "\"\n";
		}
		if (!description.empty()) {
			result += description;
			result += "\n";
		}
		result += "Available Commands:";

		// Gather all the records that are directly visible.
		Records records;
		gatherRecords(records);

		// Calculate the column widths.
		int max_name = 0;
		for (Records::iterator it = records.begin(); it != records.end(); it++) {
			int l = (*it).name.size();
			if (l > max_name)
				max_name = l; 
		}

		// Create the list of commands.
		for (Records::iterator it = records.begin(); it != records.end(); it++) {
			result += "\n  ";
			result += (*it).name;
			int pad = max_name - (*it).name.size();
			if (pad > 0)
				result += std::string(pad, ' ');
			result += "  ";
			result += (*it).description;
		}
		return result;
	}

	static ConsoleCommandGroup* make(const std::string& name, const std::string& description = "")
	{
		ConsoleCommandGroup* g = new ConsoleCommandGroup;
		g->name = name;
		g->description = description;
		return g;
	}

	virtual bool isGroup() { return true; }

private:
	struct Record { std::string name, description; };
	typedef std::vector <Record> Records;

	void gatherRecords(Records& records)
	{
		for (Commands::iterator c = commands.begin(); c != commands.end(); c++) {
			if ((*c)->name.empty() && (*c)->isGroup()) {
				((ConsoleCommandGroup*)*c)->gatherRecords(records);
			} else {
				Record r;
				r.name = (*c)->name;
				r.description = (*c)->description;
				records.push_back(r);
			}
		}
	}
};

/**
 * Wrapper around a function.
 */
template <class C> class ConsoleCommand : public AbstractConsoleCommand
{
public:
	typedef void (C::*Func)(const Args& args);
	C* object;
	Func func;

	bool execute(const Args& args)
	{
		((object)->*(func))(args);
		return true;
	}

	static ConsoleCommand<C>* make(C* object, Func func, const std::string& name, const std::string& description = "")
	{
		ConsoleCommand<C>* c = new ConsoleCommand<C>;
		c->object = object;
		c->func = func;
		c->name = name;
		c->description = description;
		return c;
	}
};

class ConsoleCommandProvider
{
public:
	virtual ConsoleCommandGroup getConsoleCommands() = 0;
};