#include "power.hpp"
#include "sensors.hpp"
#include <iostream>
#include <netinet/in.h>
#include <nlohmann/json.hpp>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

using json = nlohmann::json;
void setUpClientSocketAndSendString(std::string serializedData);

int main() {
    json sensorData = checkSensorData();
    if (sensorData != nullptr) {
        std::string serializedData = sensorData.dump();
        setUpClientSocketAndSendString(serializedData);
    }
    json powerData = checkPowerData();
    if (powerData != nullptr) {
        std::string serializedData = powerData.dump();
        setUpClientSocketAndSendString(serializedData);
    }

    return 0;
}

void setUpClientSocketAndSendString(std::string serializedData) {
    int clientSocket;
    int port = 55555;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == -1) {
        std::cout << "Error creating socket" << std::endl;
    }
    struct sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_port = htons(port);
    clientService.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(clientSocket, (sockaddr *)&clientService, sizeof(clientService)) == -1) {
        std::cout << "client coulnd't connect" << std::endl;
        close(clientSocket);
    }
    int byteCount = send(clientSocket, serializedData.c_str(), serializedData.size(), 0);

    if (byteCount < 0) {
        std::cout << "Message not sent: " << serializedData;
    }
}
