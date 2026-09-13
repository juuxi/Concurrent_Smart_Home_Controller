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

    DeviceServer server(&dashboard);
    LightDevice lightDevice(std::make_shared<DeviceServer>(server));
    lightDevice.receiveId();
    lightDevice.changeBrightness("full");
    TemperatureDevice temperatureDevice(std::make_shared<DeviceServer>(server));
    temperatureDevice.receiveId();
    temperatureDevice.changeTemperature(25);

    return app.exec();
}