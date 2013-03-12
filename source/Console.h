/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <gc_cpp.h>
#include <gc_allocator.h>
#include <boost/function.hpp>


typedef std::vector <std::string> ConsoleArgs;

/**
 * A single console command call.
 * Contains the raw user input, the input interpreted as individual command
 * arguments and an output stream that will be shown to the user as a response
 * to the command.
 */
struct ConsoleCall
{
	std::string input;
	typedef std::vector <std::string> Args;
	Args args;
	std::stringstream& out;

	ConsoleCall(std::stringstream& out) : out(out) {}
};

class AbstractConsoleCommand : public gc
{
public:
	std::string name;
	std::string parameters;
	std::string description;

	virtual bool execute(ConsoleCall& cmd) = 0;
	virtual bool isGroup() { return false; }

	virtual std::string getHelpPage() = 0;

	std::string getAbstract() const
	{
		return description.substr(0, description.find("\n"));
	}
};

class ConsoleCommandGroup : public AbstractConsoleCommand
{
public:
	typedef std::vector <AbstractConsoleCommand*, gc_allocator<AbstractConsoleCommand*> > Commands;
	Commands commands;
	
	inline AbstractConsoleCommand* add(AbstractConsoleCommand* cmd)
	{
		commands.push_back(cmd);
		return cmd;
	}
	
	inline void add(ConsoleCommandGroup grp)
	{
		ConsoleCommandGroup* ptr = new ConsoleCommandGroup;
		*ptr = grp;
		add(ptr);
	}

	bool execute(ConsoleCall& cmd)
	{
		if (cmd.args.empty())
			return false;
		ConsoleCall::Args::iterator a0 = cmd.args.begin();
		for (Commands::iterator c = commands.begin(); c != commands.end(); c++) {
			if ((*c)->name.empty()) {
				if ((*c)->execute(cmd))
					return true;
			}
			if ((*c)->name == *a0) {
				a0++;
				ConsoleCall subcmd(cmd);
				subcmd.args.assign(a0, cmd.args.end());
				return (*c)->execute(subcmd);
			}
		}
		return false;
	}

	std::string getHelpPage()
	{
		std::string result;
		if (!name.empty()) {
			result += "usage: ";
			result += name;
			result += "\n";
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
		int max_name = 0, max_parameter = 0;
		for (Records::iterator it = records.begin(); it != records.end(); it++) {
			int ln = (*it).name.size();
			int lp = (*it).parameters.size();
			if (ln > max_name) max_name = ln;
			if (lp > max_parameter) max_parameter = lp;
		}

		// Create the list of commands.
		for (Records::iterator it = records.begin(); it != records.end(); it++) {
			result += "\n  ";
			result += (*it).name;
			result += std::string(max_name - (*it).name.size(), ' ');
			result += "  ";
			result += (*it).parameters;
			result += std::string(max_parameter - (*it).parameters.size(), ' ');
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
	struct Record { std::string name, parameters, description; };
	typedef std::vector <Record> Records;

	void gatherRecords(Records& records)
	{
		for (Commands::iterator c = commands.begin(); c != commands.end(); c++) {
			if ((*c)->name.empty() && (*c)->isGroup()) {
				((ConsoleCommandGroup*)*c)->gatherRecords(records);
			} else {
				Record r;
				r.name = (*c)->name;
				r.parameters = (*c)->parameters;
				r.description = (*c)->getAbstract();
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
	typedef void (C::*Func)(ConsoleCall&);
	C* object;
	Func func;

	bool execute(ConsoleCall& cmd)
	{
		((object)->*(func))(cmd);
		return true;
	}

	std::string getHelpPage()
	{
		std::string result;
		if (!name.empty()) {
			result += "usage: ";
			result += name;
			if (!parameters.empty()) {
				result += " ";
				result += parameters;
			}
			result += "\n";
		}
		if (!description.empty()) {
			result += description;
			result += "\n";
		}
		while (result[result.size()-1] == '\n')
			result.resize(result.size()-1);
		return result;
	}

	static ConsoleCommand<C>* make(C* object, Func func, const std::string& name, const std::string& parameters = "", const std::string& description = "")
	{
		ConsoleCommand<C>* c = new ConsoleCommand<C>;
		c->object = object;
		c->func = func;
		c->name = name;
		c->parameters = parameters;
		c->description = description;
		return c;
	}
};

class ConsoleCommandProvider
{
public:
	virtual ConsoleCommandGroup getConsoleCommands() = 0;
};