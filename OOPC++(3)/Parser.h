#pragma once
#include "utility.h"
#include "DefaultConverter.h"
#include <fstream>
#include <string>


class Parser {
public:
	Parser();

	Commands parseFile(const std::string filePath);
	std::string backReadCommand(const Command&);
private:
	Logger log;
};