#include "Logger.h"

Logger::Logger() {};

void Logger::writeWarning(std::string text) {
	writeMessage(warningPrefix, text, warningSuffix);
}

void Logger::writeAnotat(std::string text) {
	writeMessage(anotatPrefix, text, anotatSuffix);
}

void Logger::writeMessage(const char* prefix, std::string text, const char* suffix) {
	err_s << prefix << " " << text << " " << suffix << std::endl;
}
