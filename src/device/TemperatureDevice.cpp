#include <TemperatureDevice.hpp>
#include <DeviceServer.hpp>

TemperatureDevice::TemperatureDevice(std::shared_ptr<DeviceServer> server): Device(server) {}

void TemperatureDevice::receiveId()
{
    id = server->giveDeviceId(DeviceType::TEMPERATURE_DEVICE);
}

void TemperatureDevice::sendData(DeviceData data)
{
    server->receiveData(id, data);
}

void TemperatureDevice::receiveData(std::string data)
{
    std::cout << data << std::endl;
}

void TemperatureDevice::changeTemperature(int value)
{
    temperature = value;
    sendData(temperature);
}