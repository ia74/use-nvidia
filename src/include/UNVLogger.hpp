#pragma once

#include "UNVGlobal.hpp"

namespace UNVLogger {
	void setLoggerMode(int status);
	void log(const char* message);
	void log(std::string message);
	char* timeToTimestamp(long long int time);
}