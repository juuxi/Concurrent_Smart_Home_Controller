#pragma once

#include <string>
#include <variant>
#include <memory>

#include <SmartHomeDashboard.hpp>

#ifndef TEST
class DeviceServer;
#else
class MockDeviceServer;
#endif

using DeviceData = std::variant<std::string, int>;

class Device
{
protected:
    #ifndef TEST
    std::shared_ptr<DeviceServer> server;
    #else
    std::shared_ptr<MockDeviceServer> server;
    #endif
    size_t id;
public:
    #ifndef TEST
    Device(std::shared_ptr<DeviceServer> server): server(server) {}
    #else
    Device(std::shared_ptr<MockDeviceServer> server): server(server) {}
    #endif
    virtual void receiveId() = 0;
    size_t getId() {return id;}

    virtual void sendData(DeviceData data) = 0;
    virtual void receiveData(DeviceData data) = 0;
    virtual ~Device() = default;
};