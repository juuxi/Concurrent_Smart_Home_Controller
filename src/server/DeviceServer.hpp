#pragma once

#include <memory>
#include <variant>
#include <iostream>

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
    int devices;

    void handlePayload(const int id, const int data);
    void handlePayload(const int id, const std::string& data);
public:
    DeviceServer();
    DeviceServer(SmartHomeDashboard* dashboard);

    int giveDeviceId();
    
    void sendData(std::shared_ptr<Device> target, std::string data);
    void receiveData(const int id, DeviceData data);
};