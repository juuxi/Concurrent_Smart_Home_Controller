#pragma once

#include <memory>
#include <variant>
#include <iostream>

#include <QWidget>
#include <boost/log/trivial.hpp>

#include <SmartHomeDashboard.hpp>

class Device;

using DeviceData = std::variant<std::string, int>;

class DeviceServer
{
private:
    SmartHomeDashboard* dashboard;

    void handlePayload(const int data);
    void handlePayload(const std::string& data);
public:
    DeviceServer() : dashboard(nullptr) {}
    DeviceServer(SmartHomeDashboard* dashboard) : dashboard(dashboard) {}
    
    void sendData(std::shared_ptr<Device> target, std::string data);
    void receiveData(DeviceData data);
};