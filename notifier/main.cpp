#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <netinet/in.h>
#include <nlohmann/json.hpp>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

using json = nlohmann::json;
std::string readEmailFromFile();

int main() {

    int serverSocket, acceptSocket;
    int port = 55555;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1) {
        std::cout << "Error creating socket" << std::endl;
        return 1;
    }
    struct sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_port = htons(port);
    service.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (bind(serverSocket, (sockaddr *)&service, sizeof(service)) == -1) {
        std::cout << "Error on bind" << std::endl;
        close(serverSocket);
        return 1;
    }
    if (listen(serverSocket, 1) == -1) {
        std::cout << "Error on listen" << std::endl;
        close(serverSocket);
        return 1;
    }
    while (true) {
        acceptSocket = accept(serverSocket, NULL, NULL);
        if (acceptSocket == -1) {
            std::cout << "Error on accept" << std::endl;
            close(serverSocket);
            return 1;
        }

        char buffer[200] = {0};
        int byteCount = recv(acceptSocket, buffer, 200, 0);

        if (byteCount > 0) {
            std::string email = readEmailFromFile();
            std::string cmd = "msmtp " + email;
            FILE *pipe = popen(cmd.c_str(), "w");
            json messageJson = json::parse(buffer);
            std::string from = messageJson["from"];
            std::string subject = messageJson["subject"];
            std::string body = messageJson["body"];
            std::string message = "From: " + from +
                                  "\n"
                                  "Subject: " +
                                  subject +
                                  "\n"
                                  "\n" +
                                  body;

            if (!pipe) {
                std::cout << "popen err" << std::endl;
                return 1;
            }

            fwrite(message.c_str(), 1, message.size(), pipe);
            int result = pclose(pipe);
            if (result != 0) {
                std::cerr << "msmtp failed: " << result << std::endl;
            }
        } else {
            std::cout << "Message not sent";
            close(acceptSocket);
        }

        char confirmation[200] = "Message Recieved";
        byteCount = send(acceptSocket, confirmation, 200, 0);
    }

    close(acceptSocket);
    close(serverSocket);

    return 0;
}

std::string readEmailFromFile() {
    std::ifstream file("/home/brabs/Projects/notify_sys/notifier/email_config.txt");
    std::string email;

    if (file.is_open()) {
        std::getline(file, email);
        file.close();
    } else {
        std::cerr << "Couldn't open config file" << std::endl;
    }
    return email;
}
