#pragma once
#include "Header.h"
#include "Config.h"
#include <sstream>
#include <fstream>
#include <chrono>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <thread>   //For a specific time we need to check mail server

class Mail {
public:
    bool isRead = 0;
    char mailPath[150];
    char sender[99];
    char subject[250];
    Mail();
    Mail(bool, const char*, const char*, const char*);
};

void recv_mail(Config configData);

void createUserFolderAndItsSubFolder(string user_addr);
std::string base64Decode(const std::string& base64String);
void saveattachments(const string& mailContent);
std::string generateUniqueFileNameWhenDownload(const std::string& filename, const std::string& filetype);
void writeToManagement(const string& filepath, const string& mailName, const string& mailContent);
string filtingMailContent(Config configData, const string& mailContent);