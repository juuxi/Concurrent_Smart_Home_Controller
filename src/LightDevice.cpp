#include <LightDevice.hpp>
#include <DeviceServer.hpp>

void LightDevice::sendData(DeviceServer& server, DeviceData data)
{
    server.receiveData(id, data);
}

void LightDevice::receiveData(std::string data)
{
    std::cout << data << std::endl;
}

void LightDevice::changeBrightness(DeviceServer& server, std::string value)
{
    brightness = value;
    sendData(server, value);
}