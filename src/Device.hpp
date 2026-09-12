#pragma once

#include <string>
#include <memory>

class DeviceServer;

class Device
{
public:
    virtual void sendData(std::unique_ptr<DeviceServer> server, std::string data) = 0;
    virtual void receiveData(std::string data) = 0;
    virtual ~Device() = default;
};