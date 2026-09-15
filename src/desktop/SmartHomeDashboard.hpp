#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <vector>

class DeviceServer;

enum class DeviceType
{
    LIGHT_DEVICE = 0,
    TEMPERATURE_DEVICE
};

class SmartHomeDashboard: public QWidget
{
    Q_OBJECT

public:
    SmartHomeDashboard(QWidget* parent = nullptr);
    void setDeviceState(int index, const std::string& text);
    
    void addDevice(int id, DeviceType type);
    QWidget* getDevice(int index);
    void setServer(std::shared_ptr<DeviceServer> newServer);

private:
    std::shared_ptr<DeviceServer> server;
    QWidget* labelContainer;
    QHBoxLayout* deviceLayout;
    std::vector<QWidget*> deviceContainers;

    using HandleTypeDependentUI = std::function<void(QGridLayout*, int)>;
    std::vector<HandleTypeDependentUI> typeDependentUIHandlers;
    void setupTypeDependentUIHandlers();

    QLabel* getValueLabel(int index);
    int getCurrentDeviceTemp(int index);

    void handleOffButton(int index);
    void handleOnButton(int index);

    void handleCoolButton(int index);
    void handleWarmButton(int index);
};