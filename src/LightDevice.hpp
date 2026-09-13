#include <Device.hpp>
#include <iostream>

class LightDevice: public Device
{
    std::string brightness;
public:
    void sendData(DeviceServer& server, DeviceData data) override;
    void receiveData(std::string data) override;

    void changeBrightness(DeviceServer& server, std::string value);

    ~LightDevice() = default;
};