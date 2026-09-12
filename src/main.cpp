#include <DeviceServer.hpp>
#include <LightDevice.hpp>
#include <SmartHomeDashboard.hpp>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    DeviceServer server;
    LightDevice lightDevice;
    lightDevice.sendData(server, "Brightness: full");

    SmartHomeDashboard dashboard;
    dashboard.show();
    return app.exec();
}