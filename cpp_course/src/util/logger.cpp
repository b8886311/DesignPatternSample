#include "logger.h"
#include <fstream>

namespace agvs{
Logger& Logger::Instance() {
    static Logger logger;
    return logger;
}
Logger& Logger::operator<<(const std::string &msg) {
    std::ofstream outFile("log.txt", std::ios::app);
    outFile << msg << std::endl;
    return *this;
}
Logger& Logger::operator<<(size_t &msg) {
    std::ofstream outFile("log.txt", std::ios::app);
    outFile << msg << std::endl;
    return *this;
}
Logger& Logger::operator<<(std::ostream& (*pf)(std::ostream&)) {
    std::ofstream outFile("log.txt", std::ios::app);
    outFile << (*pf) << std::endl;
    return *this;
}

}   //namespace