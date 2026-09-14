#include <DeviceServer.hpp>
#include <Device.hpp>

DeviceServer::DeviceServer() : dashboard(nullptr), devices(0) 
{
    boost::log::add_file_log("server_logs.log");
    setupIdHandlers();
}

DeviceServer::DeviceServer(SmartHomeDashboard* dashboard) : dashboard(dashboard), devices(0) 
{
    boost::log::add_file_log("server_logs.log");
    setupIdHandlers();
}

void DeviceServer::setupIdHandlers()
{
    idHandlers.push_back([](){return "Brightness:";});
    idHandlers.push_back([](){return "Temperature:";});
}

void DeviceServer::handlePayload(const int id, const int data)
{
    BOOST_LOG_TRIVIAL(info) << "[Server] Receive Temperature: " << data << " from device " << id;
    if (dashboard)
        dashboard->setDeviceState(id - 1, std::to_string(data));
}

void DeviceServer::handlePayload(const int id, const std::string& data)
{
    BOOST_LOG_TRIVIAL(info) << "[Server] Receive Brightness: " << data << " from device " << id;
    if (dashboard)
        dashboard->setDeviceState(id - 1, data);
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

int DeviceServer::giveDeviceId(DeviceType type)  // should be protected when multithreading is implemented
{
    devices++;
    if (dashboard)
        dashboard->addDevice(devices, idHandlers[static_cast<int>(type)]());
    return devices;
}