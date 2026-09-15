#pragma once

#include <string>
#include <variant>
#include <memory>

#include <SmartHomeDashboard.hpp>

class DeviceServer;

using DeviceData = std::variant<std::string, int>;

class Device
{
protected:
    std::shared_ptr<DeviceServer> server;
    int id;
public:
    Device(std::shared_ptr<DeviceServer> server): server(server) {}
    virtual void receiveId() = 0;

    virtual void sendData(DeviceData data) = 0;
    virtual void receiveData(DeviceData data) = 0;
    virtual ~Device() = default;
};