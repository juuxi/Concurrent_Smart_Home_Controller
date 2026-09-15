#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>

#include <vector>
#include <map>

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
    void setDeviceState(size_t id, const std::string& text);
    
    void addDevice(size_t id, DeviceType type);
    QWidget* getDevice(size_t id);
    void setServer(std::shared_ptr<DeviceServer> newServer);

private:
    std::shared_ptr<DeviceServer> server;
    QWidget* labelContainer;
    QHBoxLayout* deviceLayout;
    std::map<size_t, QWidget*> deviceContainers;

    using HandleTypeDependentUI = std::function<void(QGridLayout*, size_t)>;
    std::vector<HandleTypeDependentUI> typeDependentUIHandlers;
    void setupTypeDependentUIHandlers();

    QLabel* getValueLabel(size_t id);
    int getCurrentDeviceTemp(size_t id);

    void handleOffButton(size_t id);
    void handleOnButton(size_t id);

    void handleCoolButton(size_t id);
    void handleWarmButton(size_t id);
};