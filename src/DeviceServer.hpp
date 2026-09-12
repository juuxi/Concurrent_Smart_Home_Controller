#pragma once

#include <memory>
#include <iostream>
#include <QWidget>
#include <SmartHomeDashboard.hpp>

class Device;

class DeviceServer
{
private:
    SmartHomeDashboard* dashboard;
public:
    DeviceServer() : dashboard(nullptr) {}
    DeviceServer(SmartHomeDashboard* dashboard) : dashboard(dashboard) {}
    
    void sendData(std::shared_ptr<Device> target, std::string data);
    void receiveData(std::string data);
};