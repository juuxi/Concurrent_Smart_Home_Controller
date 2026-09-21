#include <DeviceServer.hpp>
#include <LightDevice.hpp>
#include <TemperatureDevice.hpp>

int main(int argc, char *argv[])
{
    auto server = std::make_shared<DeviceServer>();
    auto lightDevice = std::make_shared<LightDevice>(server);
    lightDevice->receiveId();
    server->addDevice(lightDevice);
    lightDevice->changeBrightness("full");
    auto temperatureDevice = std::make_shared<TemperatureDevice>(server);
    temperatureDevice->receiveId();
    server->addDevice(temperatureDevice);
    temperatureDevice->changeTemperature(15);
}