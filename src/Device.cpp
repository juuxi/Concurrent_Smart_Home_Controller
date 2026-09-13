#include <Device.hpp>
#include <DeviceServer.hpp>

void Device::receiveId() 
{
    id = server->giveDeviceId();
}
