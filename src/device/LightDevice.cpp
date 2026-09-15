#include <LightDevice.hpp>
#include <DeviceServer.hpp>

LightDevice::LightDevice(std::shared_ptr<DeviceServer> server): Device(server) {}

void LightDevice::receiveId()
{
    id = server->giveDeviceId(DeviceType::LIGHT_DEVICE);
}

void LightDevice::sendData(DeviceData data)
{
    server->receiveData(id, data);
}

void LightDevice::receiveData(DeviceData data)
{
    try
    {
        if (auto val = std::get_if<std::string>(&data))
            changeBrightness(*val);
        else
            throw std::bad_variant_access{};
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void LightDevice::changeBrightness(std::string value)
{
    brightness = value;
    sendData(brightness);
}