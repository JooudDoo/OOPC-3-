#pragma once
#include "Logger.h"
#include <string>
#include <vector>

constexpr int ERRORPARAMSEXIT = 10;
constexpr int ERRORUNKNOWNWORKINGLINECOMMAND = 3;
constexpr int ERRORMAINFILENOTOPEN = 2;
constexpr int UNKNOWN_CONVERTER = 1;
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

typedef byteV(*converterFunction)(byteV&, double, paramsArr);

typedef struct _CommandTemplate {
	CommandType type;
	converterFunction converter;
	UtilityFuncNames utilityFunc;
	std::vector<std::string> funcAliases;
	std::vector<ParamType> typeParams;
} CommandTemplate;

typedef struct _Command {
	CommandType type;
	converterFunction converter;
	UtilityFuncNames utilityFunc;
	paramsArr params;
	std::vector<ParamType> typeParams;
}Command;

using CommandTemplates = std::vector<CommandTemplate>;
using Commands = std::vector<Command>;