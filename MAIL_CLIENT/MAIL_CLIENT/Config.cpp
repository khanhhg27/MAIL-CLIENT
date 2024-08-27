#include "Config.h"

General::General() {
	this->smtpServer_port = -1;
	this->pop3Server_port = -1;
	this->autoload = -1;
}

Filter::Filter() {
	this->toProject.second = "Project";
	this->toImportant.second = "Important";
	this->toWork.second = "Work";
	this->toSpam.second = "Spam";
}

void Config::readFileXml(string filename) {
	ifstream ifs(filename);
	if (!ifs) {
		cerr << "Fail to open config file.\n";
		return;
	}

	string line;
	string content;
	while (!ifs.eof()) {
		getline(ifs, line);
		content += line;
		//content += "\n";
	}
	ifs.close();

	//Get general info
    //Get username
    size_t userNamePos = content.find("<UserName>") + sizeof("<UserName>");
    this->general.user_name = content.substr(userNamePos, content.find("&lt;", userNamePos) - userNamePos);
    //Get useraddr
    this->general.user_addr = content.substr(content.find("&lt;", userNamePos) + 4, content.find("&gt;", userNamePos) - content.find("&lt;", userNamePos) - 4);
    //Get password
    size_t passwordPos = content.find("<Password>", userNamePos) + sizeof("<Password>");
    this->general.password = content.substr(passwordPos, content.find("</Password>", passwordPos) - passwordPos);
	//Get server addr
	size_t smtpAddrPos = content.find("<MailServer>", passwordPos) + sizeof("<MailServer>");
	this->general.server_addr = content.substr(smtpAddrPos, content.find("</MailServer>", smtpAddrPos) - smtpAddrPos);
	//Get smtp port
	size_t smtpPortPos = content.find("<SMTP>", passwordPos) + sizeof("<SMTP>");
	this->general.smtpServer_port = std::stoi(content.substr(smtpPortPos, content.find("</SMTP>", smtpPortPos) - smtpPortPos));
	//Get pop3 port
	size_t pop3PortPos = content.find("<POP3>", smtpPortPos) + sizeof("<POP3>");
	this->general.pop3Server_port = std::stoi(content.substr(pop3PortPos, content.find("</POP3>", pop3PortPos) - pop3PortPos));
	//Get autoload time
	size_t autoloadPos = content.find("<AutoLoad>", pop3PortPos) + sizeof("<AutoLoad>");
	this->general.autoload = std::stoi(content.substr(autoloadPos, content.find("</AutoLoad>", autoloadPos) - autoloadPos));
	
	//Get filter info
	//ToProject
	size_t fromPos = content.find("<From>", autoloadPos) + sizeof("<From>");
	string allProjectMail = content.substr(fromPos, content.find(" - ", fromPos + 1) - fromPos);
	size_t commaPos = allProjectMail.find(", ");
	size_t previousCommaPos = 0;
	while (commaPos != std::string::npos) {
		string projectMail = allProjectMail.substr(previousCommaPos, commaPos - previousCommaPos);
		this->filter.toProject.first.push_back(projectMail);
		previousCommaPos = commaPos + 2;
		commaPos = allProjectMail.find(", ", commaPos + 1);
	}
	this->filter.toProject.first.push_back(allProjectMail.substr(previousCommaPos));

	//ToImportant
	size_t subjectPos = content.find("<Subject>", fromPos) + sizeof("<Subject>");
	string allImportantMail = content.substr(subjectPos, content.find(" - ", subjectPos + 1) - subjectPos);
	commaPos = allImportantMail.find(", ");
	previousCommaPos = 0;
	while (commaPos != std::string::npos) {
		string importantMail = allImportantMail.substr(previousCommaPos, commaPos - previousCommaPos);
		this->filter.toImportant.first.push_back(importantMail);
		previousCommaPos = commaPos + 2;
		commaPos = allImportantMail.find(", ", commaPos + 1);
	}
	this->filter.toImportant.first.push_back(allImportantMail.substr(previousCommaPos));

	//ToWork
	size_t contentPos = content.find("<Content>", subjectPos) + sizeof("<Content>");
	string allWorkMail = content.substr(contentPos, content.find(" - ", contentPos + 1) - contentPos);
	commaPos = allWorkMail.find(", ");
	previousCommaPos = 0;
	while (commaPos != std::string::npos) {
		string workMail = allWorkMail.substr(previousCommaPos, commaPos - previousCommaPos);
		this->filter.toWork.first.push_back(workMail);
		previousCommaPos = commaPos + 2;
		commaPos = allWorkMail.find(", ", commaPos + 1);
	}
	this->filter.toWork.first.push_back(allWorkMail.substr(previousCommaPos));

	//ToSpam
	size_t spamPos = content.find("<Spam>", contentPos) + sizeof("<Spam>");
	string allSpamMail = content.substr(spamPos, content.find(" - ", spamPos + 1) - spamPos);
	commaPos = allSpamMail.find(", ");
	previousCommaPos = 0;
	while (commaPos != std::string::npos) {
		string spamMail = allSpamMail.substr(previousCommaPos, commaPos - previousCommaPos);
		this->filter.toSpam.first.push_back(spamMail);
		previousCommaPos = commaPos + 2;
		commaPos = allSpamMail.find(", ", commaPos + 1);
	}
	this->filter.toSpam.first.push_back(allSpamMail.substr(previousCommaPos));


    //print general
    //this->general.printGeneralInfo();

	//print filter
	//this->filter.printFilterInfo();
}

void General::printGeneralInfo() {
	cout << "General info:\n";
    cout << this->user_name << endl;
    cout << this->user_addr << endl;
    cout << this->password << endl;
	cout << this->server_addr << endl;
	cout << this->smtpServer_port << endl;
	cout << this->pop3Server_port << endl;
	cout << this->autoload << endl;
}

void Filter::printFilterInfo() {
	cout << "From: ";
	size_t toProject_size = this->toProject.first.size();
	for (int i = 0; i < toProject_size; i++) {
		cout << this->toProject.first[i];
		if (i < toProject_size - 1) {
			cout << ", ";
		}
	}
	cout << " - To folder: " << this->toProject.second << endl;


	cout << "From: ";
	size_t toImportant_size = this->toImportant.first.size();
	for (int i = 0; i < toImportant_size; i++) {
		cout << this->toImportant.first[i];
		if (i < toImportant_size - 1) {
			cout << ", ";
		}
	}
	cout << " - To folder: " << this->toImportant.second << endl;

	cout << "From: ";
	size_t toWork_size = this->toWork.first.size();
	for (int i = 0; i < toWork_size; i++) {
		cout << this->toWork.first[i];
		if (i < toWork_size - 1) {
			cout << ", ";
		}
	}
	cout << " - To folder: " << this->toWork.second << endl;

	cout << "From: ";
	size_t toSpam_size = this->toSpam.first.size();
	for (int i = 0; i < toSpam_size; i++) {
		cout << this->toSpam.first[i];
		if (i < toSpam_size - 1) {
			cout << ", ";
		}
	}
	cout << " - To folder: " << this->toSpam.second << endl;
}