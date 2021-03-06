
#pragma once  

#include <string>
#include <fstream>
#include <iostream>
 
using namespace std;
 
class Logger
{
public:
    // log priorities
    enum Priority
    {
        DEBUG,
        CONFIG,
        INFO,
        WARNING,
        ERROR,
        OFF
    };
 
    // start/stop logging
    // - messages with priority >= minPriority will be written in log
    // - set logFile = "" to write to standard output
    static void Start(Priority minPriority, const string& logFile);
    static void Stop();
 
    // write message
    static void Write(Priority priority, const char* fmt, ...);
 
private:
    // Logger adheres to the singleton design pattern, hence the private
    // constructor, copy constructor and assignment operator.
    Logger();
    Logger(const Logger& logger) {}
    Logger& operator = (const Logger& logger) {return *this; /*remove compile warning*/ }

    void get_time(char* const szTime, const int nLen); 
    // private instance data
    bool        active;
    ofstream    fileStream;
    Priority    minPriority;
 
    // names describing the items in enum Priority
    static const string PRIORITY_NAMES[];
    // the sole Logger instance (singleton)
    static Logger instance;
};

#ifdef ENABLE_LOGGER

#define INDENT "%-22s "
#define LOGGER_START(MIN_PRIORITY, FILE) Logger::Start(MIN_PRIORITY, FILE);
#define LOGGER_STOP() Logger::Stop();
#define LOGGER_WRITE(PRIORITY, format, args...) Logger::Write(PRIORITY, "%-16s[%d]" format, __FILE__, __LINE__, ## args);
 
#else
 
#define LOGGER_START(MIN_PRIORITY, FILE)
#define LOGGER_STOP()
#define LOGGER_WRITE(PRIORITY, MESSAGE)
 
#endif
