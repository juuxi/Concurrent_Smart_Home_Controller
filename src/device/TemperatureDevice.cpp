#include <TemperatureDevice.hpp>
#include <DeviceServer.hpp>

TemperatureDevice::TemperatureDevice(std::shared_ptr<IDeviceServer> server): Device(server), temperature(20) {}

void TemperatureDevice::receiveId()
{
    id = server->giveDeviceId(DeviceType::TEMPERATURE_DEVICE);
    sendData(temperature); // Send initial temperature to the server
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
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void TemperatureDevice::changeTemperature(int value)
{
    temperature = value;
    sendData(temperature);
}

void TemperatureDevice::decreaseTemperature(int value)
{
    temperature -= value;
    sendData(temperature);
}

void TemperatureDevice::increaseTemperature(int value)
{
    temperature += value;
    sendData(temperature);
}