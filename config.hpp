#pragma once


const std::string LOG_FILE = "/etc/unv/usenvidia.log";
const std::string CONFIG_FILE = "/etc/unv/config.ini";

const char helpMessage[] = "Usage: unv [action] [options]\n"
						   "Actions:\n"
						   "  list\n"
						   "  on\n"
						   "  off\n"
						   "  status\n"
						   "  ps\n"
						   "  run\n"
						   "Options:\n"
						   "  --verbose\n"
						   "\nPlease note if you are using (unv run), you cannot apply --verbose.\n"
						   "A simple workaround is to edit your config, and change verbose in the app section to true.\n";