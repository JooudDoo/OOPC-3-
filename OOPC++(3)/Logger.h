#pragma once
#include <string>
#include <iostream>
#include <Windows.h>

#define err_s std::cerr

constexpr const char* warningPrefix = "[!!]";
constexpr const char* warningSuffix = "[!!]";

constexpr const char* anotatPrefix = ">> ";
constexpr const char* anotatSuffix = "";

class Logger {
public:
	Logger();
	Logger(const std::string);

	void writeWarning(std::string text);
	void writeAnotat(std::string text);
	void setGlobalPrefix(const std::string& prefix);

private:
	std::string globalPrefix;
	void writeMessage(const char* prefix, std::string text, const char* suffix);
	HANDLE outputHandle;
	WORD colorAtribute;

};