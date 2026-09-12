#pragma once

#include <memory>
#include <iostream>

class Device;

class DeviceServer
{
public:
    void sendData(std::shared_ptr<Device> target, std::string data);
    void receiveData(std::string data);
};