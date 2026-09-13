#include <DeviceServer.hpp>
#include <Device.hpp>

void DeviceServer::handlePayload(const int id, const int data)
{
    std::cout << data << std::endl;
    BOOST_LOG_TRIVIAL(info) << "[Server] Receive Temperature: " << data << " from device " << id;
    if (dashboard)
        dashboard->setLabelText(id - 1, std::to_string(data));
}

void DeviceServer::handlePayload(const int id, const std::string& data)
{
    std::cout << data << std::endl;
    BOOST_LOG_TRIVIAL(info) << "[Server] Receive Brightness: " << data << " from device " << id;
    if (dashboard)
        dashboard->setLabelText(id - 1, data);
}

void DeviceServer::sendData(std::shared_ptr<Device> target, std::string data)
{
    target->receiveData(data);
    BOOST_LOG_TRIVIAL(info) << "[Server] Send data \"" << data << "\" to " << target;
}

void DeviceServer::receiveData(const int id, DeviceData data)
{
    std::visit([this, id](const auto& payload) {
        handlePayload(id, payload);
    }, data);
}

int DeviceServer::giveDeviceId()  // should be protected when multithreading is implemented
{
    devices++;
    if (dashboard)
        dashboard->addLabel("Device " + std::to_string(devices));
    return devices;
}