#pragma once
#include "Header.h"
#include "Send_mail.h"
#include "Recv_mail.h"
#include <conio.h> //For _getch()

void mail_client(Config configData);
void inputString(string& toBeInputed);
void readMail(Config configData, string folder, string mailboxMes);