#include <Device.hpp>
#include <iostream>

class LightDevice : public Device
{
public:
    void sendData(std::unique_ptr<DeviceServer> server, std::string data) override;
    void receiveData(std::string data) override;
    ~LightDevice() = default;
};