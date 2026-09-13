#include <DeviceServer.hpp>
#include <Device.hpp>

void DeviceServer::sendData(std::shared_ptr<Device> target, std::string data)
{
    target->receiveData(data);
    BOOST_LOG_TRIVIAL(info) << "[Server] Send data \"" << data << "\" to " << target;
}

void DeviceServer::receiveData(std::string data)
{
    std::cout << data << std::endl;
    BOOST_LOG_TRIVIAL(info) << "[Server] Receive data \"" << data << "\"";
    if (dashboard)
        dashboard->setLabelText(data);
}