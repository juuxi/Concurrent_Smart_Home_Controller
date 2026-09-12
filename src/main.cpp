#include <DeviceServer.hpp>
#include <LightDevice.hpp>

int main()
{
    DeviceServer server;
    LightDevice lightDevice;
    lightDevice.sendData(std::make_unique<DeviceServer>(server), "Brightness: full");
    return 0;
}