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
    LightDevice lightDevice;
    lightDevice.receiveId(server);
    lightDevice.changeBrightness(server, "full");
    TemperatureDevice temperatureDevice;
    temperatureDevice.receiveId(server);
    temperatureDevice.changeTemperature(server, 25);

    return app.exec();
}