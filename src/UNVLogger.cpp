#include "include/UNVLogger.hpp"

namespace UNVLogger
{
    int loggerMode = -1;
    void setLoggerMode(int status)
    {
        loggerMode = status;
    }

    void log(const char *message)
    {
        if (loggerMode == -1)
            return;
        long long int time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        std::cout << "[" << timeToTimestamp(time) << "] " << message << std::endl;

        // write to log file
        std::ofstream logFile;
        logFile.open(LOG_FILE, std::ios_base::app);
        logFile << "[" << timeToTimestamp(time) << "] " << message << std::endl;
        logFile.close();
    }

    void log(std::string message)
    {
        log(message.c_str());
    }

    char *timeToTimestamp(long long int time)
    {
        char *timestamp = new char[20];
        time_t t = time / 1000;
        strftime(timestamp, 20, "%Y-%m-%d %H:%M:%S", localtime(&t));
        return timestamp;
    }
}