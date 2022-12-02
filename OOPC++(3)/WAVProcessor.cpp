#include "WAVProcessor.h"
#include <iostream>
#include <format>

WAVLoader::WAVLoader() : WAVpath("None"), log(), header(), fileAttached(false), WAVlenght(0), WAVreadedLenght(0) {}

WAVLoader::WAVLoader(std::string WAVpath) {
	if (!openWAV(WAVpath)) closeWAV();
}

WAVLoader::~WAVLoader() {
	if(WAV.is_open()) closeWAV();
}

bool WAVLoader::readWAVheader() {
	if (!WAV.is_open()) {
		log.writeWarning("Unable to read header");
		return false;
	}
	//TODO сделать скип всех полей до поля data
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

byte* WAVLoader::readSecond() {
	if (!fileAttached) {
		log.writeWarning("File not attached");
		return nullptr;
	}
	uint64_t bytesToRead = 0;
	if (WAVreadedLenght >= WAVlenght) {
		log.writeWarning("File ended");
		return nullptr;
	}
	if (WAVlenght - WAVreadedLenght < 1) 
		bytesToRead = (header.bitsPerSample / 8) * (uint64_t)header.numChannels * header.sampleRate * (WAVlenght - WAVreadedLenght);
	else 
		bytesToRead = (header.bitsPerSample / 8) * (uint64_t)header.numChannels * header.sampleRate;
	byte* second = new byte[bytesToRead]; // remember to delete second
	WAV.read(second, bytesToRead);
	WAVreadedLenght += bytesToRead / ((double)header.bitsPerSample / 8) * (double)header.numChannels * header.sampleRate;
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
	fileAttached = false;
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