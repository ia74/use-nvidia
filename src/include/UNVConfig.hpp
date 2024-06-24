#pragma once

#include "UNVGlobal.hpp"

namespace UNVConfig {
	extern char kernelModules[256][256];
	void loadConfig();
	void saveConfig();
	void defaultIfNotExist(std::string section, std::string key, std::string value);
}