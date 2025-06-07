#include "sensors.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
const char *cpuSensor = "Package";
const char *wifiSensor = "iwlwifi";
const char *motherBoardSensor = "acpitz";

json checkSensorData() {
    FILE *pipe = popen("sensors", "r");
    if (!pipe) {
        std::cerr << "Failed to run sensors command" << std::endl;
    }
    json sensorJson;
    sensorJson["from"] = "Sensor Data";

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        if (strstr(buffer, cpuSensor)) {
            double temp = getTemp(buffer);
            if (temp > 80) {
                sensorJson["subject"] = "CPU Temp";
                sensorJson["body"] = "CPU Temp is critical! Current Temp: " + std::to_string(temp);
                return sensorJson;
            }
        } else if (strstr(buffer, wifiSensor)) {
            fgets(buffer, sizeof(buffer), pipe);
            fgets(buffer, sizeof(buffer), pipe);
            double temp = getTemp(buffer);
            if (temp > 80) {
                sensorJson["subject"] = "WIFI Temp";
                sensorJson["body"] = "WIFI Temp is critical! Current Temp: " + std::to_string(temp);
                return sensorJson;
            }
        } else if (strstr(buffer, motherBoardSensor)) {
            fgets(buffer, sizeof(buffer), pipe);
            fgets(buffer, sizeof(buffer), pipe);
            double temp = getTemp(buffer);
            if (temp > 80) {
                sensorJson["subject"] = "Mother Board Temp";
                sensorJson["body"] = "Mother Board Temp is critical! Current Temp: " + std::to_string(temp);
                return sensorJson;
            }
        }
    }
    return nullptr;
}

double getTemp(const char *buffer) {
    const char *start = std::strpbrk(buffer, "+");
    if (!start)
        return -1;
    const char *end = std::strstr(start, "°C");
    if (!end)
        return -1;
    char tempStr[16] = {};
    std::strncpy(tempStr, start, end - start);
    tempStr[end - start] = '\0';
    return std::atof(tempStr);
}
