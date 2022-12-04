#pragma once
#include "Logger.h"
#include <string>
#include <vector>

constexpr int ERRORPARAMSEXIT = 10;
constexpr int INSTRUCTIONFILENOTOPEN = 7;

using byte = unsigned char;
using byteV = std::vector<byte>;
using paramsArr = std::vector<void*>;
using char32 = char[4];

enum CommandType {
	NOCT, // not command type
	converter,
	utility,
};

enum ParamType {
	INT_T,
	PATH_T,
};

enum UtilityFuncNames {
	NOU, //not utility
	LOAD,
	UNLOAD,
};

typedef struct _CommandTemplate {
	CommandType type;
	byteV(*converterFunc)(byteV&, double, paramsArr);
	UtilityFuncNames utilityFunc;
	std::vector<std::string> funcAliases;
	std::vector<ParamType> typeParams;
} CommandTemplate;

typedef struct _Command {
	CommandType type;
	byteV(*converterFunc)(byteV&, double, paramsArr);
	UtilityFuncNames utilityFunc;
	paramsArr params;
	std::vector<ParamType> typeParams;
}Command;

using CommandTemplates = std::vector<CommandTemplate>;
using Commands = std::vector<Command>;