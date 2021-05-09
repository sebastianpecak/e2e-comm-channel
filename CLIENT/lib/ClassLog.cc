#include "ClassLog.h"

ClassLog::ClassLog(const std::string &className) :
    _className(className),
    _isError(false)
{
}

std::string ClassLog::_GetLogEntryBegin(const std::string &logClass, const std::string &function, int line) const
{
    using namespace std;

    // Get current time.
    const auto now = time(nullptr);
    const auto tm = *localtime(&now);
    // Format log entry.
    ostringstream builder;
    builder << '[' << put_time(&tm, "%T") << '|' << logClass << '|' << _className << '|' << function << '|' << line << "] ";
    return builder.str();
}

ClassLog &ClassLog::Info(const std::string &function, int line)
{
    _isError = false;
    std::cout << std::endl << _GetLogEntryBegin("INFO", function, line);
    return *this;
}

ClassLog &ClassLog::Error(const std::string &function, int line)
{
    _isError = true;
    std::cout << std::endl << _GetLogEntryBegin("ERROR", function, line);
    return *this;
}
