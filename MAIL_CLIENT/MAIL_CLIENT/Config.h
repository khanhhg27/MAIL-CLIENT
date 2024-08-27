#pragma once
#include "Header.h"
#include <fstream>
#include <sstream>

class General {
public:
	string user_name;
	string user_addr;
	string password;
	string server_addr;
	int smtpServer_port;
	int pop3Server_port;
	int autoload;
	General();
	void printGeneralInfo();
};

class Filter {
public:
	pair<vector<string>, string> toProject;
	pair<vector<string>, string> toImportant;
	pair<vector<string>, string> toWork;
	pair<vector<string>, string> toSpam;
	Filter();
	void printFilterInfo();
};

class Config {
public:
	General general;
	Filter filter;		//What data to what folder
	Config() {};

	void readFileXml(string filename);
};