#pragma once
#include "utility.h"
#include "DefaultConverter.h"
#include <fstream>
#include <string>

static CommandTemplates availableCommands = {
	{utility, nullptr, LOAD, {"load", "stream", "open"}, {PATH_T}},
	{utility, nullptr, UNLOAD, {"outputFolder", "output", "result"}, {PATH_T}},

	{converter, silence, NOU, {"mute", "silence"}, {INT_T, INT_T}},
	{converter, mix, NOU, {"mix", "average"}, {PATH_T, INT_T}},
	{converter, normalMix, NOU, {"normalMix"}, {PATH_T, INT_T}},
};

class Parser {
public:
	Parser();

	Commands parseFile(const std::string filePath);
private:

	std::string backReadCommand(const Command&);

	Logger log;
};