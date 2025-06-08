#include "power.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
std::string batteryLifeCmd = "cat /sys/class/power_supply/BAT0/capacity";
std::string chargingStatusCmd = "cat /sys/class/power_supply/BAT0/status";

json checkPowerData() {
    FILE *batteryLifePipe = popen(batteryLifeCmd.c_str(), "r");
    if (!batteryLifePipe) {
        std::cerr << "Failed to run batteryLife command" << std::endl;
    }
    FILE *chargingStatusPipe = popen(chargingStatusCmd.c_str(), "r");
    if (!chargingStatusPipe) {
        std::cerr << "Failed to run chargingStatus command" << std::endl;
    }
    json powerJson;
    powerJson["from"] = "Power Data";

    char lifeBuffer[128];
    fgets(lifeBuffer, sizeof(lifeBuffer), batteryLifePipe);
    double battery = std::atof(lifeBuffer);
    if (battery < 20) {
        powerJson["subject"] = "Battery life is CRIT";
        powerJson["body"] = "The battery life is CRITICAL! Batter is currently " + std::to_string(battery);
        return powerJson;
    }

    char chargeBuffer[128];
    fgets(chargeBuffer, sizeof(chargeBuffer), chargingStatusPipe);
    if (strstr(chargeBuffer, "Discharging") && battery > 70 && battery < 80) {
        powerJson["subject"] = "Server unplugged charging";
        powerJson["body"] = "The server is not plugged in! Battery: " + std::to_string(battery);
        return powerJson;
    }

    return nullptr;
}
