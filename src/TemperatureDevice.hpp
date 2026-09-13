#include <Device.hpp>

class TemperatureDevice: public Device
{
    int temperature;
public:
    void sendData(DeviceServer& server, DeviceData data) override;
    void receiveData(std::string data) override;

    void changeTemperature(DeviceServer& server, int value);

    ~TemperatureDevice() = default;
};