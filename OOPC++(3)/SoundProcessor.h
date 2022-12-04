#pragma once
#include "utility.h"
#include "Logger.h"

class SoundProcessor {
public:
	SoundProcessor();

	void evaluateInsructions(const std::string filePath);
private:
	Logger log;

};