#include "Logger.h"
#include <format>
#include <random>
#include <conio.h>

Logger::Logger() : globalPrefix("None") {
	std::random_device os_seed;
	const uint_least32_t seed = os_seed();
	std::mt19937 generator(seed);
	colorAtribute = generator() % 15 + 1;
	outputHandle = GetStdHandle(STD_ERROR_HANDLE);
};

Logger::Logger(const std::string prefix) : globalPrefix(prefix) {
	std::random_device os_seed;
	const uint_least32_t seed = os_seed();
	std::mt19937 generator(seed);
	colorAtribute = generator() % 15 + 1;
	outputHandle = GetStdHandle(STD_ERROR_HANDLE);
};

void Logger::writeWarning(std::string text) {
	writeMessage(warningPrefix, text, warningSuffix);
	_getch();
}

void Logger::writeAnotat(std::string text) {
	writeMessage(anotatPrefix, text, anotatSuffix);
}

void Logger::writeMessage(const char* prefix, std::string text, const char* suffix) {
	
	DWORD l;
	CONSOLE_SCREEN_BUFFER_INFO  cinf;
	GetConsoleScreenBufferInfo(outputHandle, &cinf);
	COORD cursorPointer = cinf.dwCursorPosition;
	cursorPointer.X += 1;

	err_s << std::format("[{}]\t", globalPrefix) << prefix << " " << text << " " << suffix << std::endl;
	FillConsoleOutputAttribute(outputHandle, colorAtribute, globalPrefix.size(), cursorPointer, &l);
}

void Logger::setGlobalPrefix(const std::string& prefix) {
	globalPrefix = prefix;
}