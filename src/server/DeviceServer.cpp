#include <DeviceServer.hpp>
#include <Device.hpp>

DeviceServer::DeviceServer() : dashboard(nullptr), devices(0) 
{
    boost::log::add_file_log("server_logs.log");
    parseConfigFile("server_config.ini");
}

DeviceServer::DeviceServer(SmartHomeDashboard* dashboard) : dashboard(dashboard), devices(0) 
{
    boost::log::add_file_log("server_logs.log");
    parseConfigFile("server_config.ini");
}

void DeviceServer::parseConfigFile(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Failed to open config file at: " << filename << "\n";
        return;
    }

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) {
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);
            
            config[key] = value;
        }
    }
}

void DeviceServer::handlePayload(const int id, const int data)
{
    int temperature = data;
    int lowest_temp = config.find("LOWEST_TEMPERATURE") != config.end() ? std::stoi(config.at("LOWEST_TEMPERATURE")) : INT_MIN;
    int highest_temp = config.find("HIGHEST_TEMPERATURE") != config.end() ? std::stoi(config.at("HIGHEST_TEMPERATURE")) : INT_MAX;;
    
    BOOST_LOG_TRIVIAL(info) << "[Server] " << "Receive Temperature: " << data << " from device " << id;
    
    if (temperature < lowest_temp)
    {
        BOOST_LOG_TRIVIAL(warning) << "[Server] Temperature " << temperature << " from device " << id 
            << " is below the lowest threshold of " << lowest_temp << ", setting to lowest threshold";
        temperature = lowest_temp;
    }
    else if (temperature > highest_temp)
    {
        BOOST_LOG_TRIVIAL(warning) << "[Server] Temperature " << temperature << " from device " << id 
            << " is above the highest threshold of " << highest_temp << ", setting to highest threshold";
        temperature = highest_temp;
    }

    if (dashboard)
        dashboard->setDeviceState(id - 1, std::to_string(temperature));
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
        dashboard->addDevice(devices, type);
    return devices;
}