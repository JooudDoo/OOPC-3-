#pragma once
#include <string>
#include <fstream>
#include "Logger.h"

using byte = char;
using char32 = char[4];

#pragma pack(push, 1)
typedef struct _WAVHeader {
	char32 chunkID;
	uint32_t chunkSize;
	char32 format; // == WAVE
	
	char32 subchunk1ID; // == fmt
	uint32_t subchunk1Size; // For PCM = 16

	uint16_t audioFormat; //For PCM = 1
	uint16_t numChannels;

	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
	char32 subchunk2ID; // data section
	uint32_t subchunk2Size;
}WAVHeader;
#pragma pack(pop)

class WAVLoader {
public:
	WAVLoader();
	WAVLoader(std::string WAVpath);
	~WAVLoader();
	
	bool openWAV(std::string WAVpath);
	bool closeWAV();
	bool WAVisOpen();

	byte* readSecond();

	double getWAVlenght() const;
	WAVHeader getHeader() const;
private:
	bool readWAVheader();
	bool checkUpHeader(WAVHeader& h) const;
	bool stringComparsion(std::string, std::string) const;

	Logger log;
	std::string WAVpath;

	WAVHeader header;
	bool fileAttached;
	std::ifstream WAV;

	double WAVreadedLenght;
	double WAVlenght;
};