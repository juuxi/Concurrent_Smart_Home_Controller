#pragma once

#include <Device.hpp>
#include <iostream>

class LightDevice: public Device
{
    std::string brightness;
public:
    #ifndef TEST
    LightDevice(std::shared_ptr<DeviceServer> server);
    #else
    LightDevice(std::shared_ptr<MockDeviceServer> server);
    #endif

    void receiveId() override;
    
    void sendData(DeviceData data) override;
    void receiveData(DeviceData data) override;

    void changeBrightness(std::string value);

    ~LightDevice() = default;
};