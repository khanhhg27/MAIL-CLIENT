#include "Send_mail.h"

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
) {
    int toReceivers = (int)toReceiver.size();
    int ccReceivers = (int)ccReceiver.size();
    int bccReceivers = (int)bccReceiver.size();

    //get data
    vector<string> encodedDataS = getEncodedData(filename);
    int filenames = (int)filename.size();

    for (int i = 0; i < filenames; i++) {
        if (calBase64EncodedSize_bytes(encodedDataS[i]) > 1048576 * 3) {
            std::cout << "Oops! The " + filename[i] + " exceeding 3MB failed to be sent." << std::endl;
            encodedDataS.erase(encodedDataS.begin() + i);
            filename.erase(filename.begin() + i);
            filenames = (int)filename.size();
            i--;
        }
    }

    std::wstring stemp = std::wstring(smtp_server_addr.begin(), smtp_server_addr.end());
    LPCWSTR server_addr = stemp.c_str();

    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        exit(1);
    }

    // Create a socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        exit(1);
    }

    // Connect to the SMTP server
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(smtp_server_port);  // SMTP port
    InetPton(AF_INET, server_addr, &serverAddress.sin_addr.s_addr);  // Local server address

    if (connect(clientSocket, (sockaddr*)(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to the server." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        system("pause");
        exit(1);
    }

    char serverMessage[BUFFER_SIZE];
    memset(serverMessage, '\0', sizeof(serverMessage));
    // Receive the server's welcome message
    if (recv(clientSocket, serverMessage, sizeof(serverMessage), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to receive server message." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        system("pause");
        exit(1);
    }
    //std::cout << "SERVER: " << serverMessage;
    memset(serverMessage, '\0', sizeof(serverMessage));

    //MailFromCommand
    string EHLOCommand = "EHLO " + smtp_server_addr + "\r\n";
    string mailFromCommand = "MAIL FROM: <" + sender_addr + ">\r\n";

    //MailContent
    string to;
    for (int i = 0; i < toReceivers; i++) {
        to += toReceiver[i];
        if (i < toReceivers - 1)
            to += ", ";
    }
    string cc;
    for (int i = 0; i < ccReceivers; i++) {
        cc += ccReceiver[i];
        if (i < ccReceivers - 1)
            cc += ", ";
    }
    string boundary = "boundary";
    string getDate = getTimeUTCplus7();
    string mailMessage =
        "Content-Type: multipart/mixed; boundary=\"" + boundary + "\"\r\n"
        "Date: " + getDate + "\r\n"
        "MIME-Version: 1.0\r\n"
        "To: " + to + "\r\n"
        "Cc: " + cc + "\r\n"
        "From: " + sender_name + " <" + sender_addr + ">\r\n"
        "Subject: " + subject + "\r\n\r\n"
        "This is a multi-part message in MIME format.\r\n"
        "--" + boundary + "\r\n"
        "Content-Type: text/plain; charset=UTF-8; format=flowed\r\n\r\n"
        "" + content + "\r\n";

    string attachment;
    for (int i = 0; i < filenames; i++) {
        int pos = (int)filename[i].find(".");
        string typeOfFile = filename[i].substr(pos);

        attachment += "--" + boundary + "\r\n";
        if (typeOfFile == ".pdf") {
            attachment += "Content-Type: application/pdf; name=\"" + filename[i] + "\"\r\n";
        }
        else if (typeOfFile == ".txt") {
            attachment += "Content-Type: text/plain; charset=UTF-8; name=\"" + filename[i] + "\"\r\n";
        }
        else if (typeOfFile == ".docx") {
            attachment +=
                "Content-Type: application/vnd.openxmlformats-officedocument.wordprocessingml.document;\r\n"
                "name=\"" + filename[i] + "\"\r\n";
        }
        else if (typeOfFile == ".jpg") {
            attachment += "Content-Type: image/jpeg; name=\"" + filename[i] + "\"\r\n";
        }
        else if (typeOfFile == ".zip") {
            attachment += "Content-Type: application/x-zip-compressed; name=\"" + filename[i] + "\"\r\n";
        }
        attachment +=
            "Content-Disposition: attachment; filename=\"" + filename[i] + "\"\r\n"
            "Content-Transfer-Encoding: base64\r\n\r\n"
            + encodedDataS[i] + "\r\n";
    }

    string ending = 
        "\r\n--" + boundary + "--\r\n"
        ".\r\n";

    string mailContent = mailMessage + attachment + ending;

    //Full mail
    vector<string> mail = {
        EHLOCommand,
        mailFromCommand,
        //RCPT TO COMMAND
        "DATA\r\n",
        mailContent,
        "QUIT\r\n"
    };

    //RCPTTOCommand
    vector<string> RCPT;
    for (int i = 0; i < toReceivers; i++) {
        if (std::find(RCPT.begin(), RCPT.end(), toReceiver[i]) == RCPT.end()) {
            RCPT.push_back(toReceiver[i]);
        }
    }
    for (int i = 0; i < ccReceivers; i++) {
        if (std::find(RCPT.begin(), RCPT.end(), ccReceiver[i]) == RCPT.end()) {
            RCPT.push_back(ccReceiver[i]);
        }
    }
    for (int i = 0; i < bccReceivers; i++) {
        if (std::find(RCPT.begin(), RCPT.end(), bccReceiver[i]) == RCPT.end()) {
            RCPT.push_back(bccReceiver[i]);
        }
    }
    int RCPT_size = (int)RCPT.size();
    for (int i = 0; i < RCPT_size; i++) {
        mail.insert(mail.begin() + i + 2, "RCPT TO: <" + RCPT[i] + ">\r\n");
    }

    //Communicate with server
    int numOfMessage = (int)mail.size();
    for (int i = 0; i < numOfMessage; i++) {
        // Send message
        if (send(clientSocket, mail[i].c_str(), static_cast<int>(mail[i].length()), 0) == SOCKET_ERROR) {
            std::cerr << "Failed to send message." << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            system("pause");
            exit(1);
        }
        //std::cout << "CLIENT: " << mail[i];

        // Receive server's response
        if (recv(clientSocket, serverMessage, sizeof(serverMessage), 0) == SOCKET_ERROR) {
            std::cerr << "Failed to receive server message." << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            system("pause");
            exit(1);
        }

        //std::cout << "SERVER: " << serverMessage;
        memset(serverMessage, '\0', sizeof(serverMessage));
    }

    // Close the socket and cleanup
    closesocket(clientSocket);
    WSACleanup();
    return;
}

string base64_encode(const string& input) {
    static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    std::stringstream encoded_stream;
    size_t input_length = input.size();

    for (size_t i = 0; i < input_length; i += 3) {
        unsigned char char1 = input[i];
        unsigned char char2 = (i + 1 < input_length) ? input[i + 1] : 0;
        unsigned char char3 = (i + 2 < input_length) ? input[i + 2] : 0;

        unsigned char enc1 = char1 >> 2;
        unsigned char enc2 = ((char1 & 0x3) << 4) | (char2 >> 4);
        unsigned char enc3 = ((char2 & 0xF) << 2) | (char3 >> 6);
        unsigned char enc4 = char3 & 0x3F;

        encoded_stream << base64_chars[enc1] << base64_chars[enc2]
            << ((i + 1 < input_length) ? base64_chars[enc3] : '=')
            << ((i + 2 < input_length) ? base64_chars[enc4] : '=');
    }

    return encoded_stream.str();
}

string getTimeUTCplus7() {
    // Get the current time in UTC
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    currentTime += 7 * 3600;

    // Using gmtime_s
    std::tm gmTime;
    if (gmtime_s(&gmTime, &currentTime) != 0) {
        throw std::runtime_error("Error in gmtime_s");
    }

    // Format the date as per SMTP format
    std::stringstream smtpFormattedDate;
    smtpFormattedDate << std::put_time(&gmTime, "%a, %d %b %Y %H:%M:%S %z");

    return smtpFormattedDate.str();
}

vector<string> getEncodedData(vector<string>& filename) {
    if (filename.size() == 0) {
        return vector<string>();
    }

    vector<string> encodedDataS;
    string path = "FileToSend/";

    for (int i = 0; i < filename.size(); i++) {
        ifstream ifs(path + filename[i], std::ios::binary);
        if (!ifs) {
            std::cerr << "Fail to send attachment " << filename[i] << endl;
            filename.erase(filename.begin() + i);
            i--;
        }
        else {
            string attachmentContent((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
            ifs.close();
            string encodedAttachment = base64_encode(attachmentContent);

            int lastEle = (int)encodedAttachment.size() - 1;
            if (lastEle % 72 != 0) {
                while (lastEle % 72 != 0) {
                    lastEle--;
                }
                for (int j = lastEle; j > 0; j -= 72) {
                    encodedAttachment.insert(j, "\r\n");
                }
            }
            else {
                for (int j = lastEle - 72; j > 0; j -= 72) {
                    encodedAttachment.insert(j, "\r\n");
                }
            }

            encodedDataS.push_back(encodedAttachment);
        }
    }

    return encodedDataS;
}

unsigned long long calBase64EncodedSize_bytes(string encodedData) {
    unsigned long long n = encodedData.size();
    unsigned long long bytes = n / 4 * 3;
    if (encodedData[n - 2] == '=') return bytes - 2;
    if (encodedData[n - 1] == '=') return bytes - 1;
    return bytes;
}