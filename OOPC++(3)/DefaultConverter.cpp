#include "DefaultConverter.h"

byteV silence(byteV& sec, double position, paramsArr params) {
	if (params.size() != 2) throw std::exception("Wrong amount of arguments");

	double startConvertParam = *(double*)params[0];
	double endConvertParam = *(double*)params[1];
	if (position >= startConvertParam && position <= endConvertParam) {
		int a = sec.size();
		byteV newSec(sec.size(), 0);
		return newSec;
	}
	return sec;
}

byteV mix(byteV& sec, double position, paramsArr params) {
	if (params.size() != 2) throw std::exception("Wrong amount of arguments");

	double insertPosition = *(double*)params[0];
	WAVLoader* otherLoader = (WAVLoader*)params[1];

	if (position >= insertPosition) {
		if (otherLoader->fileEnded())
			return sec;
		byteV otherSec = otherLoader->readSecond();
		if (otherSec.size() == 0)
			return sec;
		byteV newSec = sec;
		for (size_t i = 0; i < otherSec.size()-1 && i < sec.size() - 1; i+= 2) {
			byte otherLowerByte = otherSec[i];
			byte otherUpperByte = otherSec[i+1];
			byte ourLowerByte = sec[i];
			byte ourUpperByte = sec[i+1];

			int32_t newSample = (((int16_t)otherUpperByte << 8) | otherLowerByte) + (((int16_t)ourUpperByte << 8) | ourLowerByte);

			newSample /= 2;
			newSec[i] = ((byte)newSample);
			newSec[i + 1] = ((byte)(newSample >> 8));
		}
		return newSec;
	}
	return sec;

}