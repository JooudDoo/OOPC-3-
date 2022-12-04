#include "WAVProcessor.h"
#include <iostream>
#include <format>

WAVLoader::WAVLoader() : WAVpath("None"), log("Loader"), header(), fileAttached(false), fileEndedInformation(false), WAVlenght(0), WAVreadedLenght(0) {}

WAVLoader::WAVLoader(std::string WAVpath) : WAVpath("None"), log("Loader"), header(), fileAttached(false), WAVlenght(0), WAVreadedLenght(0) {
	if (!openWAV(WAVpath)) closeWAV();
}

WAVLoader::WAVLoader(const WAVLoader& other) {
	log = other.log;
	WAVpath = other.WAVpath;
	header = other.header;
	fileAttached = other.fileAttached;
	openWAV(WAVpath);
	WAV.seekg(const_cast<WAVLoader&>(other).WAV.tellg());
	WAVreadedLenght = other.WAVreadedLenght;
	WAVlenght = other.WAVlenght;
}

WAVLoader::~WAVLoader() {
	if(WAV.is_open()) closeWAV();
}

bool WAVLoader::readWAVheader() {
	if (!WAV.is_open()) {
		log.writeWarning("Unable to read header");
		return false;
	}
	WAV.read((char*)&header, sizeof(WAVHeader));
 	while (!stringComparsion(header.subchunk2ID, "data")) {
#pragma pack(push, 1)
		struct IDsize {
			char32 id;
			uint32_t size;
		}readPattern;
#pragma pack(pop)
		char* buffer = new char[header.subchunk2Size];
		WAV.read(buffer, header.subchunk2Size);
		delete[] buffer;
		WAV.read((char*)&readPattern, sizeof(struct IDsize));
		memcpy_s(header.subchunk2ID, 4, readPattern.id, 4);
		header.subchunk2Size = readPattern.size;
	}
	if (!checkUpHeader(header)) {
		log.writeWarning("Some problems found in the file header");
		return false;
	}
	WAVlenght = header.subchunk2Size / ((double)header.bitsPerSample / 8) / header.sampleRate / header.numChannels;
	return true;
}

byteV WAVLoader::readSecond() {
	if (!fileAttached) {
		log.writeWarning("File not attached");
		return byteV();
	}
	uint64_t bytesToRead = 0;
	if (WAVreadedLenght >= WAVlenght) {
		if (!fileEndedInformation) {
			log.writeWarning(std::format("File ended [{}]", WAVpath));
			fileEndedInformation = true;
		}
		return byteV();
	}
	if (WAVlenght - WAVreadedLenght < 1) 
		bytesToRead = (header.bitsPerSample / 8) * (uint64_t)header.numChannels * header.sampleRate * (WAVlenght - WAVreadedLenght);
	else 
		bytesToRead = (header.bitsPerSample / 8) * (uint64_t)header.numChannels * header.sampleRate;
	byteV second;
	second.resize(bytesToRead, 0);
	WAV.read((char*)second.data(), bytesToRead);
	WAVreadedLenght += bytesToRead / (((double)header.bitsPerSample / 8) * (double)header.numChannels * header.sampleRate);
	return second;
}

bool WAVLoader::checkUpHeader(WAVHeader& h) const {
	if (!stringComparsion(h.format, "WAVE"))
		return false;
	if (!stringComparsion(h.subchunk1ID, "fmt "))
		return false;
	if (!stringComparsion(h.subchunk2ID, "data"))
		return false;
	if (h.audioFormat != 1)
		return false;
	if (h.subchunk1Size != 16)
		return false;
	return true;
}

WAVHeader WAVLoader::getHeader() const {
	return header;
}

double WAVLoader::getWAVlenght() const {
	return WAVlenght;
}

bool WAVLoader::fileEnded() const {
	return WAVreadedLenght == WAVlenght;
}

bool WAVLoader::openWAV(std::string WAVpath) {
	if (WAV.is_open()) {
		log.writeWarning(std::format("Unable to open file \"{}\"", WAVpath));
		return false;
	}
	WAV.open(WAVpath, std::ios::in | std::ios::binary);
	if (WAV.is_open()) {
		log.writeAnotat(std::format("File \"{}\" successfully open", WAVpath));
		WAVLoader::WAVpath = WAVpath;
		fileAttached = readWAVheader();
		return fileAttached;
	}
	return false;
}

bool WAVLoader::closeWAV() {
	std::string WAVprevPath = WAVpath;
	WAVLoader::WAVpath = "None";
	WAVreadedLenght = 0;
	WAVlenght = 0;
	fileAttached = false;
	fileEndedInformation = false;
	header = WAVHeader();
	if (WAV.is_open()) {
		WAV.close();
		log.writeAnotat(std::format("File \"{}\" successfully closed", WAVprevPath));
		return true;
	}
	log.writeAnotat(std::format("Unable to close file \"{}\"", WAVpath));
	return false;
}

bool WAVLoader::WAVisOpen() {
	return WAV.is_open();
}

bool WAVLoader::stringComparsion(const std::string f, const std::string s) const {
	for (size_t i = 0; i < s.size(); i++) {
		if (f[i] != s[i])
			return false;
	}
	return true;
}



WAVunLoader::WAVunLoader() : WAVpath("None"), log("Unloader"), fileAttached(false), headerWrited(false), WAVwritedSeconds(0) {};

WAVunLoader::WAVunLoader(std::string WAVpath) : WAVpath(WAVpath), log("Unloader"), fileAttached(false), headerWrited(false), WAVwritedSeconds(0) {
	if (!openWAV(WAVpath)) closeWAV();
}

WAVunLoader::~WAVunLoader() {
	if (WAV.is_open()) closeWAV();
}

bool WAVunLoader::openWAV(std::string WAVpath) {
	if (WAV.is_open()) {
		log.writeWarning(std::format("Unable to open file \"{}\"", WAVpath));
		return false;
	}
	WAV.open(WAVpath, std::ios::out | std::ios::binary);
	if (WAV.is_open()) {
		log.writeAnotat(std::format("File \"{}\" successfully open", WAVpath));
		WAVunLoader::WAVpath = WAVpath;
		fileAttached = true;
		return fileAttached;
	}
	return false;
}

bool WAVunLoader::closeWAV() {
	std::string WAVprevPath = WAVpath;
	WAVunLoader::WAVpath = "None";
	double WAVwritedSecondsCopy = WAVwritedSeconds;
	WAVwritedSeconds = 0;
	fileAttached = false;
	headerWrited = false;
	if (WAV.is_open()) {
		WAV.close();
		log.writeAnotat(std::format("Writed to file {} seconds", WAVwritedSecondsCopy));
		log.writeAnotat(std::format("File \"{}\" successfully closed", WAVprevPath));
		return true;
	}
	log.writeAnotat(std::format("Unable to close file \"{}\"", WAVpath));
	return false;
}

bool WAVunLoader::WAVisOpen() {
	return WAV.is_open();
}

bool WAVunLoader::writeHeader(WAVHeader h) {
	if (!fileAttached) {
		log.writeWarning("File not attached. Couldn't write header");
		return false;
	}
	if (headerWrited) {
		log.writeWarning("Header already writed");
		return false;
	}
	WAV.write((char*)&h, sizeof(WAVHeader));
	headerWrited = true;
	return headerWrited;
}

bool WAVunLoader::writeSecond(byteV sec) {
	if (!fileAttached) {
		log.writeWarning("File not attached. Couldn't write header");
		return false;
	}
	if (!headerWrited) {
		log.writeWarning("Header not writed");
		return false;
	}
	WAV.write((char*)sec.data(), sec.size());
	WAVwritedSeconds += 1;
	return true;
}