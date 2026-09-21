#pragma once

using DeviceData = std::variant<std::string, int>;

enum class DeviceType
{
    LIGHT_DEVICE = 0,
    TEMPERATURE_DEVICE
};

class IDeviceServer {
public:
    virtual ~IDeviceServer() = default;
    virtual int giveDeviceId(DeviceType type) = 0;
    virtual void receiveData(const size_t id, DeviceData data) = 0;
    virtual void transmitData(const size_t id, std::string data) = 0;
};