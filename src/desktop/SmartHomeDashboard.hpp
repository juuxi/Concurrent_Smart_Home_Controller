#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <vector>

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

private:
    QWidget* labelContainer;
    QHBoxLayout* deviceLayout;
    std::vector<QWidget*> deviceContainers;

    using HandleTypeDependentUI = std::function<void(QGridLayout*)>;
    std::vector<HandleTypeDependentUI> typeDependentUIHandlers;
    void setupTypeDependentUIHandlers();
};