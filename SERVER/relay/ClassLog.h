#ifndef CLASSLOG_H
#define CLASSLOG_H

#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

/**
 * This is basic log class to be embedded in a class.
 * It has methods for info and error logging.
 */
class ClassLog
{
    /**
     * Log class.
     */
    std::string _className;
    /**
     * Flag which determines where to put logs (cout or cerr).
     */
    bool _isError;

    /**
     * Formats and retruns log entry begin.
     * Format is: "[TIME|LEVEL|CLASS|METHOD|LINE] ".
     */
    std::string _GetLogEntryBegin(const std::string &logClass, const std::string &function, int line) const;

public:
    /**
     * Basic ctor.
     */
    explicit ClassLog(const std::string &className);
    /**
     * Begin new INFO-level log entry.
     * Returns reference to this.
     */
    ClassLog& Info(const std::string &function, int line);
    /**
     * Begin new ERROR-level log entry.
     * Returns reference to this.
     */
    ClassLog& Error(const std::string &function, int line);
    /**
     * Continue log entry began with Info or Error.
     * Returns reference to this.
     */
    template <typename T>
    ClassLog& operator<<(const T &value);
};

template <typename T>
ClassLog &ClassLog::operator<<(const T &value)
{
    if (_isError)
    {
        std::cerr << value;
        std::cerr.flush();
    }
    else
    {
        std::cout << value;
        std::cout.flush();
    }
    return *this;
}

/**
 * Helper macro for INFO-level logs.
 * Prerequisites: class must have _log field of type ClassLog.
 * Usage: LOG_INFO() << "Example log " << 1;
 */
#define LOG_INFO() (this->_log).Info(__FUNCTION__, __LINE__)
/**
 * Helper macro for ERROR-level logs.
 * Prerequisites: class must have _log field of type ClassLog.
 * Usage: LOG_ERROR() << "Example log " << 1;
 */
#define LOG_ERROR() (this->_log).Error(__FUNCTION__, __LINE__)

#endif // CLASSLOG_H
