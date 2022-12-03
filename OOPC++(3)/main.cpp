#include "Parser.h"
#include "DefaultConverter.h"
#include "WAVProcessor.h"

int main() {
	WAVLoader a("assets/district_four.wav");
	WAVLoader aOther("assets/funkorama.wav");
	WAVunLoader aUn("assets/district_four_R.wav");
	aUn.writeHeader(a.getHeader());
	Converter<2, mix> b;
	double start = 10;
	double end = 10;
	paramsArr arr = { &start, &aOther };
	for (size_t i = 0; i < a.getWAVlenght(); i++) {
		aUn.writeSecond(b.work(a.readSecond(), arr));
	}
}