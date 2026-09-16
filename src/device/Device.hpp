#pragma once

#include <string>
#include <variant>
#include <memory>

#include <SmartHomeDashboard.hpp>
#include <DeviceServerInterface.hpp>


using DeviceData = std::variant<std::string, int>;

class Device
{
protected:
    std::shared_ptr<IDeviceServer> server;
    size_t id;
public:
    Device(std::shared_ptr<IDeviceServer> server): server(server) {}
    virtual void receiveId() = 0;
    size_t getId() {return id;}

    virtual void sendData(DeviceData data) = 0;
    virtual void receiveData(DeviceData data) = 0;
    virtual ~Device() = default;
};