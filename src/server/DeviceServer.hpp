#pragma once

#include <memory>
#include <variant>
#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <fstream>

#include <QWidget>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

#include <SmartHomeDashboard.hpp>

class Device;

using DeviceData = std::variant<std::string, int>;

class DeviceServer
{
private:
    SmartHomeDashboard* dashboard;
    int devicesAmount;
    std::vector<std::shared_ptr<Device>> connectedDevices;

    void handlePayload(const int id, const int data);
    void handlePayload(const int id, const std::string& data);

    std::unordered_map<std::string, std::string> config;
    void parseConfigFile(const std::string& filename);

    void sendData(std::shared_ptr<Device> target, std::string data);
public:
    DeviceServer();
    DeviceServer(SmartHomeDashboard* dashboard);

    void addDevice(std::shared_ptr<Device> device);

    int giveDeviceId(DeviceType type);
    
    void receiveData(const int id, DeviceData data);
    void transmitData(const int index, std::string data);
};