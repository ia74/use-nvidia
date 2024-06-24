#include "include/UNVRunner.hpp"

namespace UNVRunner {
	std::string stdout_redirection = "/tmp/unv-stdout.log";
	std::string stderr_redirection = "/tmp/unv-stderr.log";

	void runCommand(std::string command) {
		std::string command_with_redirection = command + " > " + stdout_redirection + " 2> " + stderr_redirection;
		system(command_with_redirection.c_str());
	}

	void runCommand(std::string command, std::string stdout_redirection, std::string stderr_redirection) {
		std::string command_with_redirection = command + " > " + stdout_redirection + " 2> " + stderr_redirection;
		system(command_with_redirection.c_str());
	}

	int uniqueId() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	std::string getOutputOf(std::string command) {
		std::string stdout = std::to_string(uniqueId());
		runCommand(command, "/tmp/unv-stdout-" + stdout + ".log", "/tmp/unv-stderr-" + std::to_string(uniqueId()) + ".log");
		std::ifstream file("/tmp/unv-stdout-" + stdout + ".log");
		std::string output;
		std::string line;
		while(std::getline(file, line)) {
			output += line + "\n";
			UNVLogger::log("(UNVRunner: stdout) " + line);
		}
		file.close();
		return output;
	}
}