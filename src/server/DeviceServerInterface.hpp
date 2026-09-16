#pragma once
#include <SmartHomeDashboard.hpp>

using DeviceData = std::variant<std::string, int>;


class IDeviceServer {
public:
    virtual ~IDeviceServer() = default;
    virtual int giveDeviceId(DeviceType type) = 0;
    virtual void receiveData(const size_t id, DeviceData data) = 0;
    virtual void transmitData(const size_t id, std::string data) = 0;
};