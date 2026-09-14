#include <LightDevice.hpp>
#include <DeviceServer.hpp>

LightDevice::LightDevice(std::shared_ptr<DeviceServer> server): Device(server) {}

void LightDevice::sendData(DeviceData data)
{
    server->receiveData(id, data);
}

void LightDevice::receiveData(std::string data)
{
    std::cout << data << std::endl;
}

void LightDevice::changeBrightness(std::string value)
{
    brightness = value;
    sendData(value);
}