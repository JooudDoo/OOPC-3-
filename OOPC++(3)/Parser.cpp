#include "Parser.h"
#include <fstream>
#include <format>
#include <sstream>


Parser::Parser() : log("Parser") {};

Commands Parser::parseFile(const std::string filePath) {
	std::ifstream instFile(filePath);
	if (!instFile.is_open() || instFile.bad()) {
		log.writeWarning(std::format("Couldn't open instruction file {}. Exiting", filePath));
		exit(INSTRUCTIONFILENOTOPEN);
	}
	log.writeAnotat(std::format("Instruction file successfully open. [{}]", filePath));

	std::string line;
	Commands parsedCommands;
	size_t cntLines = 1;
	while (std::getline(instFile, line)) {
		std::istringstream parser(line);
		std::string commandName;
		parser >> commandName;

		CommandTemplate currentTemplate;
		currentTemplate.type = CommandType::NOCT;
		for (CommandTemplate cmdTemplate : availableCommands) {
			auto val = std::find(cmdTemplate.funcAliases.begin(), cmdTemplate.funcAliases.end(), commandName);
			if (val == cmdTemplate.funcAliases.end()) continue;
			currentTemplate = cmdTemplate;
			break;
		}
		if (currentTemplate.type == CommandType::NOCT) {
			log.writeWarning(std::format("Couldn't recognize command [command: {}, line: {}]. Skipping", commandName, cntLines));
			continue;
		}

		size_t currentIndex = parsedCommands.size();
		parsedCommands.push_back({ currentTemplate.type, currentTemplate.converterFunc,
								   currentTemplate.utilityFunc,
								   {}, currentTemplate.typeParams });
		paramsArr* params = &(parsedCommands[currentIndex].params);
		for (ParamType type : currentTemplate.typeParams) {
			switch (type) {
			case(INT_T):
			{
				int* param = new int(-1);
				parser >> *param;
				if (*param == -1) {
					log.writeWarning(std::format("Couldn't recognize parameter [command: {}, line: {}]. Exiting", commandName, cntLines));
					delete param;
					exit(ERRORPARAMSEXIT);
				}
				params->push_back((void*)param);
			}
			break;
			case(PATH_T):
			{
				std::string cmdLine = (parser.str());
				int pos = parser.tellg();
				auto leftQuot = std::find(cmdLine.begin() + pos, cmdLine.end(), '"');
				auto rightQuot = std::find(leftQuot + 1, cmdLine.end(), '"');
				if (leftQuot == cmdLine.end() || rightQuot == cmdLine.end()) {
					log.writeWarning(std::format("Couldn't recognize parameter [command: {}, line: {}]. Exiting", commandName, cntLines));
					exit(ERRORPARAMSEXIT);
				}
				parser.seekg(std::distance(cmdLine.begin(), rightQuot + 1));
				std::string* param = new std::string(leftQuot + 1, rightQuot);
				params->push_back((void*)param);
			}
			break;
			default:
				log.writeWarning(std::format("Couldn't recognize parameter type [command: {}, line: {}]. Skipping", commandName, cntLines));
				break;
			}
		}
		if (params->size() != currentTemplate.typeParams.size()) {
			log.writeWarning(std::format("Wrong amount of paramets [command: {}, line: {}]. Exiting", commandName, cntLines));
			exit(ERRORPARAMSEXIT);
		}
		cntLines += 1;
	}
	log.writeAnotat(std::format("Instruction file readed. [{}]", filePath));
	for (Command cmd : parsedCommands) {
		log.writeAnotat(std::format("Parsed command data: {}", backReadCommand(cmd)));
	}
	return parsedCommands;
}

std::string Parser::backReadCommand(const Command& cmd) {
	std::stringstream commandData;

	switch (cmd.type) {
	case(utility):
		commandData << "Utility ";
		for (CommandTemplate cmdTemplate : availableCommands) {
			if (cmdTemplate.utilityFunc == cmd.utilityFunc) {
				commandData << cmdTemplate.funcAliases[0];
				break;
			}
		}

		break;
	case(converter):
		commandData << "Converter ";
		for (CommandTemplate cmdTemplate : availableCommands) {
			if (cmdTemplate.converterFunc == cmd.converterFunc) {
				commandData << cmdTemplate.funcAliases[0];
				break;
			}
		}
		break;
	default:
		commandData << "Unknown";
		break;
	}
	commandData << " ";
	commandData << "command.\n\t\t\t";
	commandData << "Parameters: ";
	for (size_t i = 0; i < cmd.typeParams.size(); i++) {
		if (i != 0) commandData << ", ";

		switch (cmd.typeParams[i]) {
		case(PATH_T):
			commandData << *(std::string*)cmd.params[i];
			break;
		case(INT_T):
			commandData << *(int*)cmd.params[i];
			break;
		default:
			commandData << "unknown";
			break;
		}

	}
	
	return commandData.str();
}