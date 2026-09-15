#pragma once

#include <memory>
#include <variant>
#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <fstream>
#include <map>

#include <QWidget>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

#include <SmartHomeDashboard.hpp>
#include <ThreadPool.hpp>

class Device;

using DeviceData = std::variant<std::string, int>;

class DeviceServer
{
private:
    SmartHomeDashboard* dashboard;
    int devicesAmount;
    std::map<size_t, std::shared_ptr<Device>> connectedDevices;

    void handlePayload(const size_t id, const int data);
    void handlePayload(const size_t id, const std::string& data);

    std::unordered_map<std::string, std::string> config;
    void parseConfigFile(const std::string& filename);

    void sendData(std::shared_ptr<Device> target, std::string data);

    ThreadPool pool;
public:
    DeviceServer();
    DeviceServer(SmartHomeDashboard* dashboard);

    void addDevice(std::shared_ptr<Device> device);

    int giveDeviceId(DeviceType type);
    
    void receiveData(const size_t id, DeviceData data);
    void transmitData(const size_t id, std::string data);
};