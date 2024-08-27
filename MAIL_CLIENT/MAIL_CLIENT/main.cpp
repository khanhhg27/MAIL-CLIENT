#pragma comment(lib, "Ws2_32.lib")
#include "Mail_client.h"

using namespace std;

bool isContinue(bool a);

void checkMailServerEveryNs(const Config& configData);

int main() {
    //Config
    string fileConfigName = "configFile.xml";
    Config configData;
    configData.readFileXml(fileConfigName);

    //Thread
    std::thread workerThread(checkMailServerEveryNs, cref(configData));

    //Mail_client
    mail_client(configData);

    //Signal the end of worker thread
    isContinue(0);

    workerThread.join();
    cout << "Program exited\n";

    std::cout << endl;
    system("pause");
    return 0;
}

void checkMailServerEveryNs(const Config& configData) {
    while (isContinue(1)) {
        recv_mail(configData);
        std::this_thread::sleep_for(std::chrono::seconds(configData.general.autoload));
    }
}

bool isContinue(bool a) {
    static bool isContinue = 1;
    if (a == 0) {
        isContinue = 0;
    }
    return isContinue;
}