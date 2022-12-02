#pragma once
#include <string>
#include <iostream>

#define err_s std::cerr

constexpr const char* warningPrefix = "[!!]";
constexpr const char* warningSuffix = "[!!]";

constexpr const char* anotatPrefix = ">> ";
constexpr const char* anotatSuffix = "";

class Logger {
public:
	Logger();

	void writeWarning(std::string text);
	void writeAnotat(std::string text);

private:
	void writeMessage(const char* prefix, std::string text, const char* suffix);

};