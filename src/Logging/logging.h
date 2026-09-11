#pragma once

#include <string_view>

namespace logging
{

enum class Severity
{
	CRITICAL,
	WARNING,
	NOTICE,
	DEBUG
};

void create(Severity max_severity);
void create(std::string_view path, Severity max_severity);
void destroy();
void log(std::string_view message, Severity severity);

}