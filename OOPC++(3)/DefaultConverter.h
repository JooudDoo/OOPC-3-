#pragma once
#include "utility.h"
#include "WAVProcessor.h"
#include<vector>
#include <stdarg.h>

template<int cntArgs, byteV (*func)(byteV&, double, paramsArr)>
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
private:
	double WAVlenght;
	double WAVposition;

};

byteV silence(byteV& sec, double position, paramsArr params);

byteV mix(byteV& sec, double position, paramsArr params);

byteV normalMix(byteV& sec, double position, paramsArr params);