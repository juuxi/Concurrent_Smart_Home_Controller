#include <DeviceServer.hpp>
#include <LightDevice.hpp>
#include <TemperatureDevice.hpp>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

int main(int argc, char *argv[])
{
    logging::add_file_log(keywords::file_name = "server_logs.log", keywords::auto_flush = true);
    auto server = std::make_shared<DeviceServer>();
    auto lightDevice = std::make_shared<LightDevice>(server);
    lightDevice->receiveId();
    server->addDevice(lightDevice);
    lightDevice->changeBrightness("full");
    auto temperatureDevice = std::make_shared<TemperatureDevice>(server);
    temperatureDevice->receiveId();
    server->addDevice(temperatureDevice);
    temperatureDevice->changeTemperature(15);
    while(true)
    {
        ;
    }
}