#include <DeviceServer.hpp>
#include <LightDevice.hpp>

int main()
{
    DeviceServer server;
    LightDevice lightDevice;
    lightDevice.sendData(server, "Brightness: full");
    return 0;
}