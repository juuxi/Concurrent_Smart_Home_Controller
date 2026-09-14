#include <Device.hpp>
#include <iostream>

class LightDevice: public Device
{
    std::string brightness;
public:
    LightDevice(std::shared_ptr<DeviceServer> server);

    void receiveId() override;
    
    void sendData(DeviceData data) override;
    void receiveData(std::string data) override;

    void changeBrightness(std::string value);

    ~LightDevice() = default;
};