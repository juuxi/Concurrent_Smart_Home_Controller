#pragma once

#include <Device.hpp>

class TemperatureDevice: public Device
{
    int temperature;
public:
    TemperatureDevice(std::shared_ptr<IDeviceServer> server);

    void receiveId() override;

    void sendData(DeviceData data) override;
    void receiveData(DeviceData data) override;

    void changeTemperature(int value);
    void DecreaseTemperature(int value);
    void IncreaseTemperature(int value);

    ~TemperatureDevice() = default;
};