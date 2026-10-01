#include <DeviceServer.hpp>
#include <LightDevice.hpp>
#include <TemperatureDevice.hpp>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

int main(int argc, char *argv[])
{
    logging::add_file_log(keywords::file_name = "server_logs.log", keywords::auto_flush = true);
    auto server = std::make_shared<DeviceServer>();
    while (true)
    {
        ;
    }
}