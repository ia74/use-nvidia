#include "include/UNVGlobal.hpp"

using namespace std;

#include <vector>

std::string sudo(std::string cmd)
{
	return "echo 'Please enter your password. Command: " + cmd + "' && sudo " + cmd;
}


void teeAsRoot(std::string path, std::string value)
{
	std::string command = "echo \"" + value + "\" | sudo tee " + path +" &>/dev/null";
	UNVLogger::log("Running command: " + command);
	system(command.c_str());
}


void writePCIRescan()
{
	cout << "Looking for your NVIDIA GPU! You may need to enter your password. (Action: Rescanning PCI devices)" << endl; 
	teeAsRoot("/sys/bus/pci/rescan", "1");
}

void PCIDeviceGetVendor(std::string vendorID)
{
	std::string vendorName;
	if (vendorID == "0x10de")
	{
		vendorName = "NVIDIA Corporation";
	}
	else if (vendorID == "0x8086")
	{
		vendorName = "Intel Corporation";
	}
	else if (vendorID == "0x1002")
	{
		vendorName = "Advanced Micro Devices, Inc.";
	}
	else
	{
		vendorName = "Unknown";
	}
	UNVLogger::log("Vendor: " + vendorName + " (" + vendorID + ")");
}

void PCIDeviceScan()
{
	writePCIRescan();

	std::string path = "/sys/class/drm/card1/device/vendor";
	std::ifstream vendorFile(path);
	std::string vendorID;
	if (vendorFile.is_open())
	{
		std::getline(vendorFile, vendorID);
		PCIDeviceGetVendor(vendorID);
		vendorFile.close();
	}
	else
	{
		UNVLogger::log("Failed to open vendor file");
	}
}
void modprobe(std::string module)
{
	if (module.find("nouveau") != std::string::npos) {
		UNVLogger::log("Skipping module: " + module);
		return;
	}
	std::string command = "modprobe " + module;
	UNVLogger::log("Running command: " + command);
	system(sudo(command).c_str());
}

bool statusOfAllModules()
{
	PCIDeviceScan();
	bool flag = true;

	for (int i = 0; i < 256; i++)
	{
		if (UNVConfig::kernelModules[i][0] == '\0')
		{
			break;
		}
		if(UNVConfig::kernelModules[i][4] == 'e')
		{
			UNVLogger::log("Skipping module: " + std::string(UNVConfig::kernelModules[i]));
			continue;
		}
		std::string command = "modinfo " + std::string(UNVConfig::kernelModules[i]) + " >/dev/null 2>/dev/null && ! modprobe -n --first-time " + std::string(UNVConfig::kernelModules[i]) + " 2>/dev/null && echo \"1\"";
		std::string output = UNVRunner::getOutputOf(command);
		bool loaded = output != "";
		std::string lstr = loaded ? "loaded" : "not loaded";
		if (!loaded)
		{
			flag = false;
		}
		UNVLogger::log(std::string(UNVConfig::kernelModules[i]) + " is " + lstr);
	}
	return flag;
}

bool statusOfModule(std::string mod)
{
	std::string command = "modinfo " + mod + " >/dev/null 2>/dev/null && ! modprobe -n --first-time " + mod + " 2>/dev/null && echo \"1\"";
	std::string output = UNVRunner::getOutputOf(command);
	return output != "";
}

void unloadAllModules()
{
	for (int i = 0; i < 256; i++)
	{
		if (UNVConfig::kernelModules[i][0] == '\0')
		{
			break;
		}
		if (statusOfModule(std::string(UNVConfig::kernelModules[i])))
		{
			UNVLogger::log("Unloading module: " + std::string(UNVConfig::kernelModules[i]));
			modprobe("-r " + std::string(UNVConfig::kernelModules[i]));
		}
	}
}

void loadAllModules()
{
	for (int i = 0; i < 256; i++)
	{
		if (UNVConfig::kernelModules[i][0] == '\0')
		{
			break;
		}
		if (!statusOfModule(std::string(UNVConfig::kernelModules[i])))
		{
			UNVLogger::log("Loading module: " + std::string(UNVConfig::kernelModules[i]));
			modprobe(std::string(UNVConfig::kernelModules[i]));
		}
	}
	std::string command = "nvidia-modprobe";
	UNVLogger::log("Running command: " + command);
	system(sudo(command).c_str());
}

std::string readFile(std::string path)
{
	std::ifstream file(path);
	std::string content;
	if (file.is_open())
	{
		std::getline(file, content);
		file.close();
	}
	else
	{
		UNVLogger::log("Failed to open file: " + path);
	}
	return content;
}

void processList()
{
	std::string command = "lsof /dev/nvidia*";
	std::string output = UNVRunner::getOutputOf(command);
	cout << output << endl;
}

std::string getNvidiaCardID()
{
	std::string command = "lspci | grep NVIDIA | awk '{print $1}' | head -n 1";
	std::string output = UNVRunner::getOutputOf(command);
	output.erase(output.find_last_not_of(" \n\r\t") + 1);
	output[6] = '0';
	return output;
}
void on() {

	UNVLogger::log("Rescanning PCI devices");
	PCIDeviceScan();

	std::string cardID = getNvidiaCardID();
	UNVLogger::log("Telling card " + cardID + " to turn on");
	teeAsRoot("/sys/bus/pci/devices/0000:" + cardID + "/power/control", "on");

	loadAllModules();
	
	cout << "NVIDIA card is now ON" << endl;
}

void runProcessOnNvidia(std::string cmd)
{
	// TODO: make this work
	UNVLogger::log("Running command on nvidia: " + cmd);
	on();

	UNVLogger::log("Step 2: Here we go!");
	std::string fullCmd = "__NV_PRIME_RENDER_OFFLOAD=1 __VK_LAYER_NV_optimus=NVIDIA_only __GLX_VENDOR_LIBRARY_NAME=nvidia " + cmd;
	system(fullCmd.c_str());
}

void off()
{
	std::string cardID = getNvidiaCardID();
	teeAsRoot("/sys/bus/pci/devices/0000:" + cardID + "/remove", "1");
	teeAsRoot("/sys/bus/pci/devices/0000:" + cardID + "/power/control", "auto");
	unloadAllModules();
}

void status()
{
	std::string reason = "";
	bool flag = true;

	std::string cardID = getNvidiaCardID();
	std::string powerPath = "/sys/bus/pci/devices/0000:" + cardID + "/power/control";
	std::string powerStatus = readFile(powerPath);
	std::string powerStatusStr = powerStatus == "on" ? "on" : "off";
	if(powerStatus == "")
	{
		powerStatusStr = "unknown";
	}

	if(powerStatusStr == "on")
	{
		reason += "power is on";
		flag = true;
	} else {
		reason += "power is off";
		flag = false;
	}

	if (statusOfModule("nvidia"))
	{
		flag = true;
		reason += ", nvidia module is loaded";
	}
	else
	{
		reason += ", nvidia module is not loaded";
		flag = false;
	}

	if(statusOfAllModules())
	{
		reason += ", all modules are loaded";
	}
	else
	{
		reason += ", not all modules are loaded";
	}


	if (flag)
	{
		cout << "NVIDIA card is ON" << endl;
	}
	else
	{
		cout << "NVIDIA card is OFF" << endl;
	}
	cout << "Reason: " << reason << endl;
}

void invalidAction() {
	cout << helpMessage << endl;
}

int main(int argc, char **argv)
{
	int action = -1;

	if (argc > 1)
	{
		std::string arg = argv[1];
		if (arg == "list")
		{
			action = 1;
		}
		else if (arg == "on")
		{
			action = 2;
		}
		else if (arg == "off")
		{
			action = 3;
		}
		else if (arg == "status")
		{
			action = 4;
		}
		else if (arg == "ps")
		{
			action = 5;
		}
		else if (arg == "run")
		{
			action = 6;
		}
		else
		{
			invalidAction();
			return 0;
		}

		if (argc > 2)
		{
			std::string arg2 = argv[2];
			if (arg2 == "--verbose")
			{
				UNVLogger::setLoggerMode(0);
			}
		}
	}

	if (action == -1)
	{
		invalidAction();
			return 0;
	}

	UNVConfig::loadConfig();
	UNVLogger::log("Log file: " + LOG_FILE);

	switch (action)
	{
	case 2:
		on();
		break;
	case 3:
		off();
		break;
	case 4:
		status();
		break;
	case 5:
		processList();
		break;
	case 6:
		if (argc > 2)
		{
			// we want everything frmo argv[2] to the end
			std::string cmd = "";
			for (int i = 2; i < argc; i++)
			{
				cmd += std::string(argv[i]) + " ";
			}
			runProcessOnNvidia(cmd);
		}
		else
		{
			UNVLogger::log("No command specified");
			cout << "No command to run! Sadly, I cannot read your mind." << endl;
		}
		break;
	}
}