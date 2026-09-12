#include <Device.hpp>
#include <memory>
#include <iostream>

class DeviceServer
{
public:
    void sendData(std::shared_ptr<Device> target, std::string data);
    void receiveData(std::string data);
};