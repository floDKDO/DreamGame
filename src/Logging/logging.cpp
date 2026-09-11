#include "logging.h"

#include <ostream>
#include <fstream>
#include <iostream>

namespace logging
{

std::ostream* output_ = nullptr;
bool is_dynamically_allocated_;
Severity max_severity_;

void create(Severity max_severity)
{
	output_ = &std::cout;
	is_dynamically_allocated_ = false;
	max_severity_ = max_severity;
}

void create(std::string_view path, Severity max_severity)
{
	output_ = new std::ofstream(std::string(path));
	is_dynamically_allocated_ = true;
	max_severity_ = max_severity;
}

void destroy()
{
	if(is_dynamically_allocated_)
	{
		delete output_;
	}
}

void log(std::string_view message, Severity severity)
{
	if(output_ != nullptr && max_severity_ <= severity)
	{
		std::ostream& output_ref = *output_;

		if(severity == Severity::CRITICAL)
		{
			output_ref << "\033[1m\033[31m" << "[CRITICAL] " << message << "\033[0m" << "\n";
		}
		else if(severity == Severity::WARNING)
		{
			output_ref << "\033[1m\033[33m" << "[WARNING] " << message << "\033[0m" << "\n";
		}
		else if(severity == Severity::NOTICE)
		{
			output_ref << "\033[1m\033[34m" << "[NOTICE] " << message << "\033[0m" << "\n";
		}
		else if(severity == Severity::DEBUG)
		{
			output_ref << "\033[1m\033[30m" << "[DEBUG] " << message << "\033[0m" << "\n";
		}
	}
}

}