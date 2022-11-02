#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include "Note.h"

class Application
{
private:
	static const std::unordered_map<std::string, int> command;
	static void trim(std::string&);

	std::string req;
	bool isRunning;
	std::vector<Note> collection;

	int checkRequest(const std::string);
	void printCommandList();
	void parseRequestAdd();
	void parseRequestDel();
	void parseRequestGet();
	void parseRequestUpd();
	void parseRequestFilter();

	Application() : req("-"), isRunning(false) {}

public:
	static Application& app();

	void open();
	void close();
	void run();
};