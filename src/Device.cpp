#include <Device.hpp>
#include <DeviceServer.hpp>

void Device::receiveId(DeviceServer& server) 
{
    id = server.giveDeviceId();
}
