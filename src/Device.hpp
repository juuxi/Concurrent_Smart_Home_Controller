#pragma once

#include <string>
#include <variant>

class DeviceServer;

using DeviceData = std::variant<std::string, int>;

class Device
{
public:
    virtual void sendData(DeviceServer& server, DeviceData data) = 0;
    virtual void receiveData(std::string data) = 0;
    virtual ~Device() = default;
};