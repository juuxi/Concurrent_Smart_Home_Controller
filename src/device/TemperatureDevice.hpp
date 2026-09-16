#pragma once

#include <Device.hpp>

class TemperatureDevice: public Device
{
    int temperature;
public:
    #ifndef TEST
    TemperatureDevice(std::shared_ptr<DeviceServer> server);
    #else
    TemperatureDevice(std::shared_ptr<MockDeviceServer> server);
    #endif

    void receiveId() override;

    void sendData(DeviceData data) override;
    void receiveData(DeviceData data) override;

    void changeTemperature(int value);

    ~TemperatureDevice() = default;
};