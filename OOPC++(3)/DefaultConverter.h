#pragma once
#include "utility.h"
#include "WAVProcessor.h"
#include<vector>
#include <stdarg.h>

template<int cntArgs, converterFunction func>
class Converter {
public:
	Converter() : WAVlenght(0), WAVposition(0) {};

	byteV work(byteV second, paramsArr params) {
		if (params.size() != cntArgs) {
			throw std::exception("Wrong amount of arguments");
		}
		WAVposition += 1;
		return func(second, WAVposition, params);;
	}

	void setWAVlenght(const double& lenght) {
		WAVlenght = lenght;
	}
	converterFunction getFunction() const {
		return func;
	}
private:
	double WAVlenght;
	double WAVposition;

};

byteV silence(byteV& sec, double position, paramsArr params);

byteV mix(byteV& sec, double position, paramsArr params);

byteV normalMix(byteV& sec, double position, paramsArr params);

using ConverterSilence = Converter<2, silence>;
using ConverterMix = Converter<2, mix>;
using ConverternormalMix = Converter<2, normalMix>;

static CommandTemplates availableCommands = {
	{utility, nullptr, LOAD, {"load", "stream", "open"}, {PATH_T}},
	{utility, nullptr, UNLOAD, {"outputFolder", "output", "result"}, {PATH_T}},

	{converter, silence, NOU, {"mute", "silence"}, {INT_T, INT_T}},
	{converter, mix, NOU, {"mix", "average"}, {PATH_T, INT_T}},
	{converter, normalMix, NOU, {"normalMix"}, {PATH_T, INT_T}},
};