#include "log.h" 
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
 
void Logger::Write(Priority priority, const string& message)
{
    if (instance.active && priority >= instance.minPriority)
    {
        // identify current output stream
        ostream& stream
            = instance.fileStream.is_open() ? instance.fileStream : std::cout;
 
        stream  << PRIORITY_NAMES[priority]
                << ": "
                << message
                << endl;
    }
}


#if 0
int main()
{
    LOGGER_START(Logger::DEBUG, "");
    LOGGER_WRITE(Logger::DEBUG, "trying to allocate buffer");
    char sentence[]="This is a sentence with 7 tokens";
    cout << "The string to be tokenized is:\n" << sentence << "\n\nThe tokens are:\n\n";
    char *tokenPtr=strtok(sentence, " ");
    while(tokenPtr!=NULL) {
        cout<<tokenPtr<<'\n';
        tokenPtr=strtok(NULL," ");
    }
    //cout << "After strtok,sentence=" << tokenPtr<<endl;

    cout << "sen: " <<  sentence << " \n";
    return 0;
}
#endif

