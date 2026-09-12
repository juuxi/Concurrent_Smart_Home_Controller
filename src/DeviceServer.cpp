#include <DeviceServer.hpp>
#include <Device.hpp>

void DeviceServer::sendData(std::shared_ptr<Device> target, std::string data)
{
    target->receiveData(data);
}

void DeviceServer::receiveData(std::string data)
{
    std::cout << data << std::endl;
    if (dashboard)
        dashboard->setLabelText(data);
}