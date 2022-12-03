#include "Logger.h"
#include <format>

Logger::Logger() : globalPrefix("None") {};

Logger::Logger(const std::string prefix) : globalPrefix(prefix) {};

void Logger::writeWarning(std::string text) {
	writeMessage(warningPrefix, text, warningSuffix);
}

void Logger::writeAnotat(std::string text) {
	writeMessage(anotatPrefix, text, anotatSuffix);
}

void Logger::writeMessage(const char* prefix, std::string text, const char* suffix) {
	err_s << std::format("[{}]\t", globalPrefix) << prefix << " " << text << " " << suffix << std::endl;
}

void Logger::setGlobalPrefix(const std::string& prefix) {
	globalPrefix = prefix;
}