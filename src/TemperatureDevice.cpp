#include <TemperatureDevice.hpp>
#include <DeviceServer.hpp>

void TemperatureDevice::sendData(DeviceServer& server, std::string data)
{
    server.receiveData(data);
}

void TemperatureDevice::receiveData(std::string data)
{
    std::cout << data << std::endl;
}