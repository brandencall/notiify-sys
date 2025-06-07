#pragma once

#include <nlohmann/json_fwd.hpp>
nlohmann::json checkSensorData();
double getTemp(const char *buffer);
