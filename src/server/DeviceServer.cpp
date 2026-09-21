#include <DeviceServer.hpp>
#include <Device.hpp>
#include <TemperatureDevice.hpp>

DeviceServer::DeviceServer() : devicesAmount(0), pool(8)
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

void DeviceServer::handlePayload(const size_t id, const int data)
{
    //sendDataToDashboard();
    int temperature = data;
    int lowest_temp = config.find("LOWEST_TEMPERATURE") != config.end() ? std::stoi(config.at("LOWEST_TEMPERATURE")) : INT_MIN;
    int highest_temp = config.find("HIGHEST_TEMPERATURE") != config.end() ? std::stoi(config.at("HIGHEST_TEMPERATURE")) : INT_MAX;;
    
    BOOST_LOG_TRIVIAL(info) << "[Server] " << "Receive Temperature: " << data << " from device " << id;
    
    if (temperature < lowest_temp)
    {
        BOOST_LOG_TRIVIAL(warning) << "[Server] Temperature " << temperature << " from device " << id 
            << " is below the lowest threshold of " << lowest_temp << ", setting to lowest threshold";
        
        sendData(connectedDevices[id], std::to_string(lowest_temp));
        temperature = lowest_temp;
    }
    else if (temperature > highest_temp)
    {
        BOOST_LOG_TRIVIAL(warning) << "[Server] Temperature " << temperature << " from device " << id 
            << " is above the highest threshold of " << highest_temp << ", setting to highest threshold";
        
        sendData(connectedDevices[id], std::to_string(highest_temp));
        temperature = highest_temp;
    }

    //if (dashboard)
        //dashboard->setDeviceState(id, std::to_string(temperature));
}

void DeviceServer::handlePayload(const size_t id, const std::string& data)
{
    BOOST_LOG_TRIVIAL(info) << "[Server] Receive Brightness: " << data << " from device " << id;
    //sendDataToDashboard();
    //if (dashboard)
        //dashboard->setDeviceState(id, data);
}

void DeviceServer::addDevice(std::shared_ptr<Device> device)
{
    connectedDevices[device->getId()] = device;
}

void DeviceServer::sendData(std::shared_ptr<Device> target, std::string data)
{
    try {
        if (dynamic_cast<TemperatureDevice*>(target.get())) {
            target->receiveData(std::stoi(data));
        }
        else {
            target->receiveData(data);
        }
    }
    catch (const std::exception& e) {
        BOOST_LOG_TRIVIAL(error) << "[Server] Failed to send data to device: " << e.what();
    }
    BOOST_LOG_TRIVIAL(info) << "[Server] Send data \"" << data << "\" to " << target;
}

void DeviceServer::receiveData(const size_t id, DeviceData data)
{
    pool.enqueue([this, id, data](){
        std::visit([this, id](const auto& payload) {
            handlePayload(id, payload);
        }, data);
    });
}

void DeviceServer::transmitData(const size_t id, std::string data)
{
    pool.enqueue([this, id, data](){
        sendData(connectedDevices[id], data);
    });
}

int DeviceServer::giveDeviceId(DeviceType type)
{
    devicesAmount++;
    std::string message = "{id: " + std::to_string(devicesAmount) + ", type: " + std::to_string(static_cast<int>(type)) + "}";
    RabbitMqClient::sendData(config.at("DASHBOARD_HOST").c_str(), std::stoi(config.at("DASHBOARD_PORT")),
                             config.at("DASHBOARD_EXCHANGE").c_str(), config.at("DASHBOARD_ROUTING_KEY").c_str(),
                             message.c_str());
    // if (dashboard)
    //    dashboard->addDevice(devicesAmount, type);
    return devicesAmount;
}