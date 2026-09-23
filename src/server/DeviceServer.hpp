#pragma once

#include <memory>
#include <variant>
#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <fstream>
#include <map>

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include "utils.h"

#include "Messages.pb.h"

#include <ThreadPool.hpp>
#include <DeviceServerInterface.hpp>
#include <RabbitMqClient.hpp>

class Device;

class DeviceServer : public IDeviceServer
{
private:
    //std::jthread listenThread;

    int devicesAmount;
    std::map<size_t, std::shared_ptr<Device>> connectedDevices;

    void handlePayload(const size_t id, const int data);
    void handlePayload(const size_t id, const std::string& data);

    std::unordered_map<std::string, std::string> config;
    void parseConfigFile(const std::string& filename);

    void sendData(std::shared_ptr<Device> target, std::string data);
    void sendMessageToDashboard(const Messages::UpdateDataMessage& message);

    ThreadPool pool;
public:
    DeviceServer();

    void addDevice(std::shared_ptr<Device> device);

    int giveDeviceId(DeviceType type) override;
    
    void receiveData(const size_t id, DeviceData data) override;
    void transmitData(const size_t id, std::string data) override;
};