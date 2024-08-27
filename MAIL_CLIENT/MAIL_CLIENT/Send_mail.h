#pragma once
#include "Header.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <WinSock2.h>
#include <Ws2tcpip.h>

void send_mail(
	string sender_name,
	string sender_addr,
	int smtp_server_port,
	string smtp_server_addr,
	vector<string> toReceiver,
	vector<string> ccReceiver,
	vector<string> bccReceiver,
	vector<string> filename,
	string subject,
	string content
);
string base64_encode(const string&);
string getTimeUTCplus7();
vector<string> getEncodedData(vector<string>& filename);
unsigned long long calBase64EncodedSize_bytes(string encodedData);