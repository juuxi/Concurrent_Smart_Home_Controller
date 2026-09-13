#include <TemperatureDevice.hpp>
#include <DeviceServer.hpp>

void TemperatureDevice::sendData(DeviceServer& server, DeviceData data)
{
    server.receiveData(id, data);
}

void TemperatureDevice::receiveData(std::string data)
{
    std::cout << data << std::endl;
}

void TemperatureDevice::changeTemperature(DeviceServer& server, int value)
{
    temperature = value;
    sendData(server, temperature);
}