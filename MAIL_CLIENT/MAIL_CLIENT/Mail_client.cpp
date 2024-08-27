#include "Mail_client.h"

void mail_client(Config configData) {
	//Create user mailbox if user is new
	createUserFolderAndItsSubFolder(configData.general.user_addr);

	string welcomeMes =
		"Welcome to mail client program user " + configData.general.user_name + ""
		"<" + configData.general.user_addr + ">.\nPlease select your option:\n"
		"1. To send mail\n"
		"2. To check mail received\n"
		"3. Exit\n"
		"You choose (1, 2 or 3): ";
	std::cout << welcomeMes;

	bool isLooping = 1;
	while (isLooping) {
		char key = _getch();
		if (key == '1') {
			//Neccesary container for sending mail
			vector<string> toReceiver;
			vector<string> ccReceiver;
			vector<string> bccReceiver;
			vector<string> filename;

			std::cout << "1\n";
			std::cout << "Enter your email information: (if don't have, press Enter)\n";
			std::cout << "(Make sure to add \", \" to seperate your mail recipients\n"
						 "to avoid undesired program behaviour.)\n";

			string allToRecp;
			std::cout << "To: ";
			inputString(allToRecp);
			if (!allToRecp.empty()) {
				size_t previousCommaPos = 0;
				size_t commaPos = allToRecp.find(", ");
				while (commaPos != std::string::npos) {
					string toRecp = allToRecp.substr(previousCommaPos, commaPos - previousCommaPos);
					toReceiver.push_back(toRecp);
					previousCommaPos = commaPos + 2;
					commaPos = allToRecp.find(", ", commaPos + 1);
				}
				toReceiver.push_back(allToRecp.substr(previousCommaPos));
			}

			string allCCRecp;
			std::cout << "CC: ";
			inputString(allCCRecp);
			if (!allCCRecp.empty()) {
				size_t previousCommaPos = 0;
				size_t commaPos = allCCRecp.find(", ");
				while (commaPos != std::string::npos) {
					string ccRecp = allCCRecp.substr(previousCommaPos, commaPos - previousCommaPos);
					ccReceiver.push_back(ccRecp);
					previousCommaPos = commaPos + 2;
					commaPos = allCCRecp.find(", ", commaPos + 1);
				}
				ccReceiver.push_back(allCCRecp.substr(previousCommaPos));
			}
			
			string allBCCRecp;
			std::cout << "BCC: ";
			inputString(allBCCRecp);
			if (!allBCCRecp.empty()) {
				size_t previousCommaPos = 0;
				size_t commaPos = allBCCRecp.find(", ");
				while (commaPos != std::string::npos) {
					string bccRecp = allBCCRecp.substr(previousCommaPos, commaPos - previousCommaPos);
					bccReceiver.push_back(bccRecp);
					previousCommaPos = commaPos + 2;
					commaPos = allBCCRecp.find(", ", commaPos + 1);
				}
				bccReceiver.push_back(allBCCRecp.substr(previousCommaPos));
			}
			
			if (toReceiver.empty() && ccReceiver.empty() && bccReceiver.empty()) {
				std::cout << "No recipient data received.\n\n";
				std::cout << welcomeMes;
				isLooping = 1;
			}
			else {
				string subject;
				std::cout << "Subject: ";
				inputString(subject);
				if (subject.empty()) {
					subject = "<No subject>";
				}
				string content;
				std::cout << "(Enter your mail content, end with a line containing only '.')\n";
				std::cout << "Content:\n";
				while (true) {
					string line;
					getline(cin, line);
					if (line == ".") {
						break;
					}
					else {
						line += "\r\n";
						content += line;
					}
				}

				std::cout << "Do you want to attach file(s) to the mail? (1.Yes, 2.No): ";
				while (true) {
					int attFile_key = _getch();
					if (attFile_key == '2') {
						std::cout << "2\n";
						break;
					}
					else if (attFile_key == '1') {
						std::cout << "1\n";
						int numOfAttFile = 0;
						while (true) {
							std::cout << "Enter the number of attached file: ";
							string tmp;
							getline(cin, tmp);
							stringstream ss(tmp);
							if (ss >> numOfAttFile) {
								break;
							}
							else {
								std::cout << "Invalid input.\n";
							}
						}
						std::cout << "(Make sure you put the file wanting to send in MAIL_CLIENT/MAIL_CLIENT/FileToSend)\n";
						string fname;
						for (int i = 0; i < numOfAttFile; i++) {
							std::cout << "Enter file no." << i + 1 << "'s name with extension: ";
							getline(cin, fname);
							filename.push_back(fname);
						}
						break;
					}
				}

				send_mail(
					configData.general.user_name,
					configData.general.user_addr,
					configData.general.smtpServer_port,
					configData.general.server_addr,
					toReceiver,
					ccReceiver,
					bccReceiver,
					filename,
					subject,
					content
				);

				std::cout << "\nMail successfully sent.\n\n";
				std::cout << welcomeMes;
				isLooping = 1;
			}
		}
		else if (key == '2') {
			std::cout << "2\n";
			string mailboxMes =
				"Here is your folders in your mailbox:\n"
				"1. Inbox\n"
				"2. Project\n"
				"3. Important\n"
				"4. Work\n"
				"5. Spam\n"
				"What folder you want to check for email (1, 2, 3, 4, 5 or Enter to escape): ";
			std::cout << mailboxMes;
			while (true) {
				char mailboxKey = _getch();
				if (mailboxKey == 13) {	//Enter key
					std::cout << "<Enter>\n";
					break;
				}
				else if (mailboxKey == '1') {
					std::cout << "1\n";
					readMail(configData, "Inbox", mailboxMes);
				}
				else if (mailboxKey == '2') {
					std::cout << "2\n";
					readMail(configData, "Project", mailboxMes);
				}
				else if (mailboxKey == '3') {
					std::cout << "3\n";
					readMail(configData, "Important", mailboxMes);
				}
				else if (mailboxKey == '4') {
					std::cout << "4\n";
					readMail(configData, "Work", mailboxMes);
				}
				else if (mailboxKey == '5') {
					std::cout << "5\n";
					readMail(configData, "Spam", mailboxMes);
				}
			}
			std::cout << welcomeMes;
		}
		else if (key == '3') {
			std::cout << "3\n";
			std::cout << "\nWaiting for exiting\n";
			isLooping = 0;
		}
		else{
			isLooping = 1;
			continue;
		}
	}
}

void inputString(string& toBeInputed) {
	while (true) {
		char isEnterKey = _getch();
		if (isEnterKey == 13) {
			std::cout << "<Enter>\n";
			break;
		}
		else if (isEnterKey < 33 || isEnterKey == 127) {
			continue;
		}
		else {
			std::cout << isEnterKey;
			getline(cin, toBeInputed);
			toBeInputed = isEnterKey + toBeInputed;
			break;
		}
	}
}

void readMail(Config configData, string foldername, string mailboxMes) {
	ifstream ifs("Mailbox/" + configData.general.user_addr + "/" + foldername + "/management.dat", std::ios::binary);
	if (!ifs) {
		std::cout << "Cannot open " + foldername + "/manament.dat\n";
		throw exception();
	}
	ifs.seekg(0L, ios::end);
	size_t numOfMail = ifs.tellg() / sizeof(Mail);
	ifs.seekg(0L, ios::beg);

	if (numOfMail > 0) {
		vector<Mail> listOfMail;
		listOfMail.resize(numOfMail);

		for (size_t i = 0; i < numOfMail; i++) {
			ifs.read((char*)&listOfMail[i], sizeof(Mail));
		}
		ifs.close();

		std::cout << "There are " << numOfMail << " mails in the list of email in " + foldername + " folder:\n";
		for (size_t i = 0; i < numOfMail; i++) {
			std::cout << i + 1 << ". ";
			if (listOfMail[i].isRead == 0) {
				std::cout << "(Not read yet) ";
			}
			std::cout << "<" << listOfMail[i].sender << ">, <" << listOfMail[i].subject << ">\n";
		}
		std::cout << endl;

		while (true) {
			std::cout << "Which mail do you want to read (press Enter to escape, or 0 to review (and renew) mail list in this folder): ";
			string tmp;
			inputString(tmp);
			if (tmp.empty()) {	//Enter pressed
				std::cout << mailboxMes;
				break;
			}
			int option = 0;
			stringstream ss(tmp);
			if (ss >> option) {
				if (option == 0) {
					ifs.open("Mailbox/" + configData.general.user_addr + "/" + foldername + "/management.dat", std::ios::binary);
					if (!ifs) {
						std::cout << "Cannot open manament.dat to read\n";
						throw exception();
					}
					ifs.seekg(0L, ios::end);
					size_t numOfMail_ = ifs.tellg() / sizeof(Mail);
					ifs.seekg(0L, ios::beg);

					if (numOfMail_ > numOfMail) {	//There are mails added
						for (int i = 0; i < numOfMail_; i++) {
							Mail mail;
							ifs.read((char*)&mail, sizeof(Mail));
							if (i >= numOfMail) {
								listOfMail.push_back(mail);
							}
						}
						numOfMail = numOfMail_;
					}
					else if (numOfMail_ < numOfMail) {
						std::cerr << "Some thing wrong with management.dat\n";
						throw exception();
					}
					ifs.close();

					std::cout << "There are " << numOfMail << " mails in the list of email in Inbox folder:\n";
					for (size_t i = 0; i < numOfMail; i++) {
						std::cout << i + 1 << ". ";
						if (listOfMail[i].isRead == 0) {
							std::cout << "(Not read yet) ";
						}
						std::cout << "<" << listOfMail[i].sender << ">, <" << listOfMail[i].subject << ">\n";
					}
				}
				else if (option > numOfMail || option < 0) {
					std::cout << "Invalid input\n";
				}
				else {
					string mailPath = listOfMail[option - 1].mailPath;
					listOfMail[option - 1].isRead = 1;
					ifstream ifs(mailPath);
					if (!ifs) {
						std::cerr << "Cannot open mail.\n";
						throw exception();
					}
					string fullMail;
					string from;
					string to;
					string cc;
					string subject;
					string content;
					while (!ifs.eof()) {
						string line;
						getline(ifs, line);

						if (to.empty()) {
							if (line.find("To: ") != std::string::npos) {
								to = line;
							}
						}
						if (cc.empty()) {
							if (line.find("Cc: ") != std::string::npos) {
								cc = line;
							}
						}
						if (from.empty()) {
							if (line.find("From: ") != std::string::npos) {
								from = line;
							}
						}
						if (subject.empty()) {
							if (line.find("Subject: ") != std::string::npos) {
								subject = line;
							}
						}

						fullMail += line + '\n';
					}
					ifs.close();

					size_t pos = fullMail.find("Content-Type: text/plain; charset=UTF-8; format=flowed");
					pos = fullMail.find("\n\n", pos + 1) + 2;
					//If there any attachment
					if (fullMail.find("Content-Disposition: attachment", pos + 1) != std::string::npos) {
						content = fullMail.substr(pos, fullMail.find("\n--", pos) - pos);
					}
					else {
						content = fullMail.substr(pos);
						size_t pos_ = content.find("\n--");
						if (pos_ != std::string::npos && content.find("--\n", pos_ + 3) != std::string::npos) {
							content = content.substr(0, pos_);
						}
						content.pop_back();
					}
					std::cout << "Mail no." << option << "'s content is:\n";
					std::cout << from << endl;
					if (to.size() > 4)
						std::cout << to << endl;
					if (cc.size() > 4)
						std::cout << cc << endl;
					if (subject.size() > 9) {
						std::cout << subject << endl;
					}
					else {
						std::cout << "Subject: <No subject>\n";
					}
					if (content.size() > 0) {
						std::cout << "Content:\n" << content << "\n\n";
					}
					else {
						std::cout << "Content:\n" << "<No content>\n\n";
					}
					//Save attachment if there any
					saveattachments(fullMail);
				}
			}
			else {	//User not enter an integer
				std::cout << "Invalid input\n";
			}
		}
		//Update read status
		ofstream ofs("Mailbox/" + configData.general.user_addr + "/" + foldername + "/management.dat", std::ios::binary);
		if (!ofs) {
			std::cerr << "Canonot open management.dat\n";
			throw exception();
		}
		for (int i = 0; i < listOfMail.size(); i++) {
			ofs.write((char*)&listOfMail[i], sizeof(Mail));
		}
		ofs.close();

	}
	else {	//numOfMail <= 0
		std::cout << "There is no mail found here.!\n";
		std::cout << mailboxMes;
		ifs.close();
	}
}