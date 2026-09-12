#include <LightDevice.hpp>
#include <DeviceServer.hpp>

void LightDevice::sendData(DeviceServer& server, std::string data)
{
    server.receiveData(data);
}

void LightDevice::receiveData(std::string data)
{
    std::cout << data << std::endl;
}