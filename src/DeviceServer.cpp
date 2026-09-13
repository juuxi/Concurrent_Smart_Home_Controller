#include <DeviceServer.hpp>
#include <Device.hpp>

void DeviceServer::handlePayload(const int data)
{
    std::cout << data << std::endl;
    BOOST_LOG_TRIVIAL(info) << "[Server] Receive Temperature: " << data;
    if (dashboard)
        dashboard->setLabelText(std::to_string(data));
}

void DeviceServer::handlePayload(const std::string& data)
{
    std::cout << data << std::endl;
    BOOST_LOG_TRIVIAL(info) << "[Server] Receive Brightness: " << data;
    if (dashboard)
        dashboard->setLabelText(data);
}

void DeviceServer::sendData(std::shared_ptr<Device> target, std::string data)
{
    target->receiveData(data);
    BOOST_LOG_TRIVIAL(info) << "[Server] Send data \"" << data << "\" to " << target;
}

void DeviceServer::receiveData(DeviceData data)
{
    std::visit([this](const auto& payload) {
        handlePayload(payload);
    }, data);
}