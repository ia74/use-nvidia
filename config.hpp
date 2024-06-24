#pragma once


const std::string LOG_FILE = "/etc/unv/usenvidia.log";
const std::string CONFIG_FILE = "/etc/unv/config.ini";

const char helpMessage[] = "Usage: unv [action] [options]\n"
						   "Actions:\n"
						   "  on - Just turn your NVIDIA GPU on.\n"
						   "  off - Just turn your NVIDIA GPU off.\n"
						   "  status - Query the status of your NVIDIA GPU.\n"
						   "  ps - List all the processes running on your NVIDIA GPU.\n"
						   "  run - Run a program on your NVIDIA GPU.\n"
						   "Options:\n"
						   "  --verbose - Get output from UNVLogger.\n"
						   "\nPlease note if you are using (unv run), you cannot apply --verbose.\n"
						   "A simple workaround is to edit your config, and change verbose in the app section to true.\n";