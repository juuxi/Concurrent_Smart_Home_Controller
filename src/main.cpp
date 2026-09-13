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
    LightDevice lightDevice(server);
    lightDevice.receiveId();
    lightDevice.changeBrightness("full");
    TemperatureDevice temperatureDevice(server);
    temperatureDevice.receiveId();
    temperatureDevice.changeTemperature(25);

    return app.exec();
}