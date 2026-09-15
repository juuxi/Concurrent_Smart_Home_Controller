#include <DeviceServer.hpp>
#include <LightDevice.hpp>
#include <TemperatureDevice.hpp>
#include <SmartHomeDashboard.hpp>

#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile styleFile("styles.qss");
    if(styleFile.open(QIODevice::ReadOnly))
    {
        QTextStream textStream(&styleFile);
        QString styleSheet = textStream.readAll();
        styleFile.close();
        app.setStyleSheet(styleSheet);
    }

    SmartHomeDashboard dashboard;
    dashboard.show();

    auto server = std::make_shared<DeviceServer>(&dashboard);
    dashboard.setServer(server);
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