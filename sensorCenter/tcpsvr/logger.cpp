#include <time.h>  

#include<sys/timeb.h>
#include<time.h>
#include <stdarg.h>
 
#include "logger.h"
// --------------------------------------
// static members initialization
// --------------------------------------
 
const string Logger::PRIORITY_NAMES[] =
{
    "DEBUG",
    "CONFIG",
    "INFO",
    "WARNING",
    "ERROR"
};
 
Logger Logger::instance;
 
 
// --------------------------------------
// function implementations
// --------------------------------------
 
Logger::Logger() : active(false) {}
 
void Logger::Start(Priority minPriority, const string& logFile)
{
    instance.active = true;
    instance.minPriority = minPriority;
    if (logFile != "")
    {
        instance.fileStream.open(logFile.c_str());
    }
}
 
void Logger::Stop()
{
    instance.active = false;
    if (instance.fileStream.is_open())
    {
        instance.fileStream.close();
    }
}

void Logger::get_time(char* const  szTime, const int nLen)
{ 
    struct timeb now;
    struct tm   *time_now;

    ftime(&now);
    time_now = localtime(&now.time);
    sprintf(szTime,"%.4d-%.2d-%.2d %.2d:%.2d:%.2d.%.3d",
                    1900+ time_now->tm_year,
                    1 + time_now->tm_mon,
                    time_now->tm_yday,
                    time_now->tm_hour,
                    time_now->tm_min,
                    time_now->tm_sec,
                    now.millitm);  
}

void Logger::Write(Priority priority, const char* fmt, ...)
{  
#define SIZE_OF_BUF 2048
#define SIZE_OF_TIME 24
    if (instance.active && priority >= instance.minPriority) 
    {
        char szTime[SIZE_OF_TIME];

        va_list ap;
        static char buf[SIZE_OF_BUF];
        char *bptr = buf;

        va_start(ap, fmt);
        vsprintf(bptr, fmt, ap);
        va_end(ap);

        instance.get_time(szTime, SIZE_OF_TIME);
        ostream& stream
            = instance.fileStream.is_open() ? instance.fileStream : std::cout;

        char szLog[512]={0};
        snprintf(szLog, 512, "%s %-8s%s", szTime, PRIORITY_NAMES[priority].c_str(), buf);
        stream << szLog << endl;
        /*
           stream << szTime << "  "
           << PRIORITY_NAMES[priority] << "  "
           << buf
           << endl; 
           */
    }
}

//
//// EOF
