#include "SoundProcessor.h"
#include "DefaultConverter.h"
#include "Parser.h"
#include <format>
#include <stdio.h>

static std::string getMiddleWAVName(const size_t count) {
	return std::format("Intermediate{}", count);
}

SoundProcessor::SoundProcessor() : log("SoundProcessor") {};

void SoundProcessor::evaluateInsructions(const std::string filePath) {
	Parser instrParser;
	Commands cmds = instrParser.parseFile(filePath);
	
	WAVLoader mainWAV;
	WAVunLoader mainOutputWAV;
	size_t countTrash = 0;
	WAVunLoader middleWAV(getMiddleWAVName(countTrash));

	for (Command cmd : cmds) {
		switch (cmd.type) {
		case(utility):
		{
			switch (cmd.utilityFunc) {
			case(LOAD):
			{
				if (mainWAV.WAVisOpen()) {
					log.writeWarning("Main WAV already opened. Skiping");
					log.writeWarning(std::format("Command: {}", instrParser.backReadCommand(cmd)));
					break;
				}
				mainWAV.openWAV(*(std::string*)cmd.params[0]);
				if (!mainWAV.WAVisOpen()) {
					log.writeWarning("Couldn't open file matched as main. Exiting");
					log.writeWarning(std::format("Command: {}", instrParser.backReadCommand(cmd)));
					exit(ERRORMAINFILENOTOPEN);
				}
				log.writeAnotat("Successfully opened WAV matched as main");
				log.writeAnotat(std::format("Command: {}", instrParser.backReadCommand(cmd)));
				delete cmd.params[0];
				break;
			}
			case(UNLOAD):
			{
				if (mainOutputWAV.WAVisOpen()) {
					log.writeWarning("Main output WAV already opened. Skiping");
					log.writeWarning(std::format("Command: {}", instrParser.backReadCommand(cmd)));
					break;
				}
				mainOutputWAV.openWAV(*(std::string*)cmd.params[0]);
				if (!mainWAV.WAVisOpen()) {
					log.writeWarning("Couldn't open file matched as main output. Exiting");
					log.writeWarning(std::format("Command: {}", instrParser.backReadCommand(cmd)));
					exit(ERRORMAINFILENOTOPEN);
				}
				log.writeAnotat("Successfully opened WAV matched as main output");
				log.writeAnotat(std::format("Command: {}", instrParser.backReadCommand(cmd)));
				delete cmd.params[0];
				break;
			}
			default:
			{
				log.writeWarning("Unknown utility command pushed to working line. Exiting");
				log.writeWarning(std::format("Command: {}", instrParser.backReadCommand(cmd)));
				exit(ERRORUNKNOWNWORKINGLINECOMMAND);
			}
			}
			break;
		}
		case(converter):
			if (cmd.converter == ConverterSilence().getFunction()) {
				log.writeAnotat("Right now using Silence converter");
				ConverterSilence converter;
				middleWAV.writeHeader(mainWAV.getHeader());
				for (size_t i = 0; i < mainWAV.getWAVlenght(); i++) {
					middleWAV.writeSecond(converter.work(mainWAV.readSecond(), cmd.params));
				}
				middleWAV.closeWAV();
				mainWAV.closeWAV();
				mainWAV.openWAV(getMiddleWAVName(countTrash));
				middleWAV.openWAV(getMiddleWAVName(++countTrash));
				log.writeAnotat("Silence converter ended work");
				for (auto param : cmd.params) {
					delete param;
				}
			}
			else if (cmd.converter == ConverterMix().getFunction()) {
				log.writeAnotat("Right now using Mix converter");
				ConverterMix converter;
				WAVLoader* additionFile = new WAVLoader(*(std::string*)cmd.params[0]);
				delete cmd.params[0];
				cmd.params[0] = additionFile;
				middleWAV.writeHeader(mainWAV.getHeader());
				for (size_t i = 0; i < mainWAV.getWAVlenght(); i++) {
					middleWAV.writeSecond(converter.work(mainWAV.readSecond(), cmd.params));
				}
				additionFile->closeWAV();
				middleWAV.closeWAV();
				mainWAV.closeWAV();
				mainWAV.openWAV(getMiddleWAVName(countTrash));
				middleWAV.openWAV(getMiddleWAVName(++countTrash));
				log.writeAnotat("Mix converter ended work");
				for (auto param : cmd.params) {
					delete param;
				}
			}
			else if (cmd.converter == ConverternormalMix().getFunction()) {
				log.writeAnotat("Right now using normalMix converter");
				ConverternormalMix converter;
				WAVLoader* additionFile = new WAVLoader(*(std::string*)cmd.params[0]);
				delete cmd.params[0];
				cmd.params[0] = additionFile;
				middleWAV.writeHeader(mainWAV.getHeader());
				for (size_t i = 0; i < mainWAV.getWAVlenght(); i++) {
					middleWAV.writeSecond(converter.work(mainWAV.readSecond(), cmd.params));
				}
				additionFile->closeWAV();
				middleWAV.closeWAV();
				mainWAV.closeWAV();
				mainWAV.openWAV(getMiddleWAVName(countTrash));
				middleWAV.openWAV(getMiddleWAVName(++countTrash));
				log.writeAnotat("normalMix converter ended work");
				for (auto param : cmd.params) {
					delete param;
				}
			}
			else {
				log.writeWarning("Unknown converter pushed to working line. Skiping");
				log.writeWarning(std::format("Command: {}", instrParser.backReadCommand(cmd)));
				break;
			}
			break;
		default:
		{
			log.writeWarning("Unknown command pushed to working line. Exiting");
			log.writeWarning(std::format("Command: {}", instrParser.backReadCommand(cmd)));
			exit(ERRORUNKNOWNWORKINGLINECOMMAND);
		}
		}
	}

	middleWAV.closeWAV();

	log.writeAnotat("Recording of the final file has begun");
	if (!mainOutputWAV.WAVisOpen()) {
		log.writeWarning("The output file is not specified. The default value will be used: result.wav");
		mainOutputWAV.openWAV("result.wav");
	}
	mainOutputWAV.writeHeader(mainWAV.getHeader());
	for (size_t i = 0; i < mainWAV.getWAVlenght(); i++) {
		mainOutputWAV.writeSecond(mainWAV.readSecond());
	}
	log.writeAnotat("The recording of the final file is finished");
	mainOutputWAV.closeWAV();
	mainWAV.closeWAV();


	log.writeAnotat("The removal of intermediate files has begun");
	for (size_t i = 0; i <= countTrash; i++) {
		remove(getMiddleWAVName(i).c_str());
	}
	log.writeAnotat("Intermediate files have been successfully deleted");
}