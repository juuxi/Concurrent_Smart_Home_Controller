#include <SmartHomeDashboard.hpp>
#include <QVBoxLayout>

SmartHomeDashboard::SmartHomeDashboard(QWidget* parent)
    : QWidget(parent)
{
    setWindowState(Qt::WindowMaximized);
    setWindowTitle("Smart Home Dashboard");

    labelContainer = new QWidget(this);
    deviceLayout = new QHBoxLayout(labelContainer);
    deviceLayout->setContentsMargins(10, 10, 10, 10);
    deviceLayout->setSpacing(10);
    deviceLayout->addStretch();

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(labelContainer);
    layout->addStretch();
    setLayout(layout);
}

void SmartHomeDashboard::addDevice(const std::string& text)
{
    QGridLayout* container = new QGridLayout();
    QLabel* deviceLabel = new QLabel(QString::fromStdString(text));
    deviceLabel->setStyleSheet("background-color: #f0f0f0; padding: 8px; border-radius: 4px; border: 1px solid #ddd;");
    container->addWidget(deviceLabel);

    device_containers.push_back(container);
    
    deviceLayout->insertLayout(deviceLayout->count() - 1, container);
}

QGridLayout* SmartHomeDashboard::getDevice(int index)
{
    if (index >= 0 && index < static_cast<int>(device_containers.size()))
        return device_containers[index];
    return nullptr;
}

void SmartHomeDashboard::setDeviceState(int index, const std::string& text)
{
    auto container = getDevice(index);
    auto label = qobject_cast<QLabel*>(container->itemAt(0)->widget());
    if (label)
        label->setText(QString::fromStdString(text));
}