#include <DeviceServer.hpp>
#include <LightDevice.hpp>
#include <SmartHomeDashboard.hpp>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SmartHomeDashboard dashboard;
    dashboard.show();

    DeviceServer server(&dashboard);
    LightDevice lightDevice;
    lightDevice.sendData(server, "Brightness: full");

    return app.exec();
}