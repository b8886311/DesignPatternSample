#ifndef AGVS_INCLUDE_UTIL_LOGGER_H
#define AGVS_INCLUDE_UTIL_LOGGER_H
#include <string>
#include <ostream>
namespace agvs {
class Logger {
public:
    static Logger& Instance();
    Logger& operator<<(const std::string &msg);
    Logger& operator<<(size_t &msg);
    Logger& operator<<(std::ostream& (*pf)(std::ostream&));
private:
    Logger() = default;
};
}

#endif
