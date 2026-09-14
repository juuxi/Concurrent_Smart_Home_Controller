#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <vector>

class SmartHomeDashboard: public QWidget
{
    Q_OBJECT

public:
    SmartHomeDashboard(QWidget* parent = nullptr);
    void setDeviceState(int index, const std::string& text);
    
    void addDevice(const std::string& text);
    QGridLayout* getDevice(int index);

private:
    QWidget* labelContainer;
    QHBoxLayout* deviceLayout;
    std::vector<QGridLayout*> device_containers;

    enum class DeviceLayoutItem
    {
        ID_LABEL = 0,
        MEASURMENT_LABEL,
        MESAURMENT_VALUE,
        BUTTON_LAYOUT
    };
};