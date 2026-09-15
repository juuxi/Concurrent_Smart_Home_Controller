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

void TemperatureDevice::receiveData(DeviceData data)
{
    try
    {
        if (auto val = std::get_if<int>(&data))
            changeTemperature(*val);
        else
            throw std::bad_variant_access{};
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void TemperatureDevice::changeTemperature(int value)
{
    temperature = value;
    sendData(temperature);
}