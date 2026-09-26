#pragma once

#include <Device.hpp>
#include <iostream>

class LightDevice: public Device
{
    std::string brightness;
public:
    LightDevice(std::shared_ptr<IDeviceServer> server);

    void receiveId() override;
    
    void sendData(DeviceData data) override;
    void receiveData(DeviceData data) override;

    void changeBrightness(std::string value);

    void lightOn();
    void lightHalf();
    void lightOff();

    ~LightDevice() = default;
};