#pragma once

#include <string>

class DeviceServer;

class Device
{
public:
    virtual void sendData(DeviceServer& server, std::string data) = 0;
    virtual void receiveData(std::string data) = 0;
    virtual ~Device() = default;
};