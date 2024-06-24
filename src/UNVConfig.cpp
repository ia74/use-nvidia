#include <cstring>

#include "include/UNVConfig.hpp"


void mkListFromCommas(const char* str, char list[][256]) {
    int i = 0; // Index for input string
    int j = 0; // Index for the current string in list
    int k = 0; // Index for the list of strings

    // Skip initial spaces
    while (str[i] == ' ') i++;

    while (str[i] != '\0') {
        if (str[i] == ',') {
            list[k][j] = '\0'; // Terminate current string
            k++; // Move to the next string
            j = 0; // Reset index for the new string
            i++; // Move past the comma
            // Skip spaces after comma
            while (str[i] == ' ') i++;
            continue; // Skip the rest of the loop
        }
        list[k][j] = str[i]; // Copy character
        j++;
        i++;
    }
    list[k][j] = '\0'; // Terminate the last string
}

namespace UNVConfig {
	mINI::INIFile configFile(CONFIG_FILE);
	mINI::INIStructure config;
	bool canWrite = true;
	char kernelModules[256][256];
	void saveConfig() {
		if(canWrite) {
			configFile.write(config);
			UNVLogger::log("Config file saved");
		}
	}
	void defaultIfNotExist(std::string section, std::string key, std::string value) {
		if(config[section][key] == "") {
			UNVLogger::log("Key " + key + " not found in section " + section + ", setting to default value");
			config[section][key] = value;
			saveConfig();
		}
	}
	void loadConfig() {
		std::ifstream cfgTest(CONFIG_FILE);
		if (!cfgTest.good()) {
			UNVLogger::log("Config file not found, creating one");
			std::ofstream cfgFile(CONFIG_FILE);
			cfgFile.close();
		}

		std::ifstream logTest(LOG_FILE);
		if (!logTest.good()) {
			UNVLogger::log("Log file not found, creating one");
			std::ofstream logFile(LOG_FILE);
			logFile.close();
		}

		configFile.read(config);
		if(config["app"]["verbose"] == "true") {
			UNVLogger::setLoggerMode(0);
		}

		if(config["app"]["version"] == "") {
			UNVLogger::log("Version not found, setting to app version");
			config["app"]["version"] = APP_VERSION;
			saveConfig();
		} else {
			UNVLogger::log("App version: " + APP_VERSION_STR);
			UNVLogger::log("Config version: " + config["app"]["version"]);
			if(config["app"]["version"] != APP_VERSION) {
				UNVLogger::log("Configuration version mismatch. Exiting! Please update the config file manually.");
				config["app"]["version"] = APP_VERSION;
				canWrite = false;
				exit(1);
			}
		}

		if(config["app"]["read-only"] == "true") {
			UNVLogger::log("Config file is read-only, will not write any changes to config file.");
			canWrite = false;
		}

		defaultIfNotExist("unv", "kernel_modules", "nouveau, nvidia, nvidia_drm modeset=1 fbdev=1, nvidia_uvm, nvidia_modeset");
		defaultIfNotExist("unv:device", "classes", "display");
		defaultIfNotExist("unv:device", "vendors", "nvidia");
		defaultIfNotExist("unv:egl_vendor", "path", "/usr/share/glvnd/egl_vendor.d/10_nvidia.json");
		defaultIfNotExist("unv:egl_vendor", "apply", "true");
		defaultIfNotExist("unv", "kill_running_before_unloading", "true");

		mkListFromCommas(config["unv"]["kernel_modules"].c_str(), kernelModules);

	}
}