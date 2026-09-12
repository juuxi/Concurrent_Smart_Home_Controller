#include <string>

class Device
{
public:
    virtual void sendData(std::string data);
    virtual void receiveData(std::string data);
};