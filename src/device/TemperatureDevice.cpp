#include <TemperatureDevice.hpp>
#include <DeviceServer.hpp>

#ifndef TEST
TemperatureDevice::TemperatureDevice(std::shared_ptr<DeviceServer> server): Device(server) {}
#else
#include <gtest/gtest.h>
#include <gmock/gmock.h>
class MockDeviceServer {
public:
    MOCK_METHOD(int, giveDeviceId, (DeviceType type));
    MOCK_METHOD(void, receiveData, (const size_t id, DeviceData data));
};

TemperatureDevice::TemperatureDevice(std::shared_ptr<MockDeviceServer> server): Device(server) {};
#endif

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