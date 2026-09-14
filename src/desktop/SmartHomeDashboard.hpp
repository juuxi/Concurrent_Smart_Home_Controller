#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
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
    
    void addDevice(int id, const std::string& text);
    QWidget* getDevice(int index);

private:
    QWidget* labelContainer;
    QHBoxLayout* deviceLayout;
    std::vector<QWidget*> deviceContainers;

    enum class DeviceLayoutItem
    {
        ID_LABEL = 0,
        MEASURMENT_LABEL,
        MESAURMENT_VALUE,
        BUTTON_LAYOUT
    };
};