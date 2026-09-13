#pragma once

#include <string>
#include <variant>
#include <memory>

class DeviceServer;

using DeviceData = std::variant<std::string, int>;

class Device
{
protected:
    std::shared_ptr<DeviceServer> server;
    int id;
public:
    Device(std::shared_ptr<DeviceServer> server): server(server) {}
    void receiveId();

    virtual void sendData(DeviceData data) = 0;
    virtual void receiveData(std::string data) = 0;
    virtual ~Device() = default;
};