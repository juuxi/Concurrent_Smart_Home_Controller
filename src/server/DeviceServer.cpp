#include <DeviceServer.hpp>
#include <Device.hpp>
#include <TemperatureDevice.hpp>
#include <LightDevice.hpp>

DeviceServer::DeviceServer() : devicesAmount(0), pool(8)
{
    boost::log::add_file_log("server_logs.log");
    parseConfigFile("server_config.ini");
    setupListenThread();
    setupPollQueueThread();
}

void DeviceServer::setupListenThread()
{
    listenThread = std::jthread(
        [config = config, messageQueue = std::ref(messageQueue)]()
        {
            try
            {
                RabbitMqClient::listen(config.at("FROM_DASHBOARD_HOST").c_str(), std::stoi(config.at("FROM_DASHBOARD_PORT")),
                                       config.at("FROM_DASHBOARD_EXCHANGE").c_str(), config.at("FROM_DASHBOARD_ROUTING_KEY").c_str(), 
                                       config.at("FROM_DASHBOARD_QUEUE").c_str(), messageQueue);
            }
            catch (const std::exception& e)
            {
                std::cerr << "Listening thread failed: " << e.what() << '\n';
            }
        }
    );
}

void DeviceServer::setupPollQueueThread()
{
    pollQueueThread = std::jthread(
        [this]()
        {
            while (true)
            {
                if (!messageQueue.empty())
                {
                    auto message = messageQueue.front();
                    messageQueue.pop();

                    Messages::ChangeDataMessage changeDataMessage;
                    if (!changeDataMessage.ParseFromString(message))
                    {
                        std::cerr << "Failed to parse message from dashboard: " << message << std::endl;
                        continue;
                    }
                    
                    if (changeDataMessage.has_change_temperature())
                    {
                        if (changeDataMessage.change_temperature().has_decrease_temperature())
                        {
                            int id = changeDataMessage.change_temperature().decrease_temperature().id();
                            auto device = dynamic_cast<TemperatureDevice*>(connectedDevices[id].get());
                            if (device)
                                device->DecreaseTemperature(1);
                            BOOST_LOG_TRIVIAL(info) << "[Server] Received request to decrease temperature of device " << id;
                        }
                        else if (changeDataMessage.change_temperature().has_increase_temperature())
                        {
                            int id = changeDataMessage.change_temperature().increase_temperature().id();
                            auto device = dynamic_cast<TemperatureDevice*>(connectedDevices[id].get());
                            if (device)
                                device->IncreaseTemperature(1);
                            BOOST_LOG_TRIVIAL(info) << "[Server] Received request to increase temperature of device " << id;
                        }
                    }
                    else if (changeDataMessage.has_change_light())
                    {
                        if (changeDataMessage.change_light().has_light_on())
                        {
                            int id = changeDataMessage.change_light().light_on().id();
                            auto device = dynamic_cast<LightDevice*>(connectedDevices[id].get());
                            if (device)
                                device->lightOn();
                            BOOST_LOG_TRIVIAL(info) << "[Server] Received request to turn on light of device " << id;
                        }
                        else if (changeDataMessage.change_light().has_light_half())
                        {
                            int id = changeDataMessage.change_light().light_half().id();
                            auto device = dynamic_cast<LightDevice*>(connectedDevices[id].get());
                            if (device)
                                device->lightHalf();
                            BOOST_LOG_TRIVIAL(info) << "[Server] Received request to half light of device " << id;
                        }
                        else if (changeDataMessage.change_light().has_light_off())
                        {
                            int id = changeDataMessage.change_light().light_off().id();
                            auto device = dynamic_cast<LightDevice*>(connectedDevices[id].get());
                            if (device)
                                device->lightOff();
                            BOOST_LOG_TRIVIAL(info) << "[Server] Received request to turn off light of device " << id;
                        }
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    );
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

void DeviceServer::sendMessageToDashboard(const Messages::UpdateDataMessage& message)
{
    size_t size = message.ByteSizeLong();
    char *buffer = new char[size];
    message.SerializeToArray(buffer, size);

    RabbitMqClient::sendData(config.at("TO_DASHBOARD_HOST").c_str(), std::stoi(config.at("TO_DASHBOARD_PORT")),
                             config.at("TO_DASHBOARD_EXCHANGE").c_str(), config.at("TO_DASHBOARD_ROUTING_KEY").c_str(),
                             buffer);
}

void DeviceServer::handlePayload(const size_t id, const int data)
{
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
    Messages::UpdateDataMessage message;
    message.mutable_device_measurement()->set_id(id);
    message.mutable_device_measurement()->mutable_temperature_device_measurement()->set_temperature(temperature);

    sendMessageToDashboard(message);
}

void DeviceServer::handlePayload(const size_t id, const std::string& data)
{
    BOOST_LOG_TRIVIAL(info) << "[Server] Receive Brightness: " << data << " from device " << id;
    Messages::UpdateDataMessage message;
    message.mutable_device_measurement()->set_id(id);
    message.mutable_device_measurement()->mutable_light_device_measurement()->set_brightness(data);

    sendMessageToDashboard(message);
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
    Messages::UpdateDataMessage message;
    message.mutable_new_device_data()->set_id(devicesAmount);
    message.mutable_new_device_data()->set_type(Messages::NewDeviceData_DeviceType(static_cast<int>(type) + 1));  // Adjusting to 1-based enum in protobuf

    sendMessageToDashboard(message);

    return devicesAmount;
}