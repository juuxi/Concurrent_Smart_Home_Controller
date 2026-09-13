#include <Device.hpp>

class TemperatureDevice: public Device
{
public:
    void sendData(DeviceServer& server, std::string data) override;
    void receiveData(std::string data) override;
    ~TemperatureDevice() = default;
};