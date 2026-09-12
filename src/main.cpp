#include <DeviceServer.hpp>

int main()
{
    DeviceServer server;
    server.receiveData("Hello from Concurrent Smart Home Controller");
    return 0;
}