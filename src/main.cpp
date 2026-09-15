#include <DeviceServer.hpp>
#include <LightDevice.hpp>
#include <TemperatureDevice.hpp>
#include <SmartHomeDashboard.hpp>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SmartHomeDashboard dashboard;
    dashboard.show();

    auto server = std::make_shared<DeviceServer>(&dashboard);
    auto lightDevice = std::make_shared<LightDevice>(server);
    lightDevice->receiveId();
    server->addDevice(lightDevice);
    lightDevice->changeBrightness("full");
    auto temperatureDevice = std::make_shared<TemperatureDevice>(server);
    temperatureDevice->receiveId();
    server->addDevice(temperatureDevice);
    temperatureDevice->changeTemperature(15);

    return app.exec();
}