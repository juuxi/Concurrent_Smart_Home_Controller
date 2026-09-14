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

void SmartHomeDashboard::addDevice(int id, DeviceType type)
{
    // !! REWRITE !!
    std::string text;
    std::vector<QPushButton*> btns;
    if (type == DeviceType::LIGHT_DEVICE)
    {
        text = "Brightness:";
        auto off_btn = new QPushButton("Off");
        btns.push_back(off_btn);
        auto on_btn = new QPushButton("On");
        btns.push_back(on_btn);
    }
    if (type == DeviceType::TEMPERATURE_DEVICE)
    {
        text = "Temperature:";
        auto cool_btn = new QPushButton("Cool");
        btns.push_back(cool_btn);
        auto warm_btn = new QPushButton("Warm");
        btns.push_back(warm_btn);
    }
    // !! REWRITE !!

    QWidget* container = new QWidget();
    container->setStyleSheet("border-radius: 4px; border: 1px solid #ccc");
    QGridLayout* layout = new QGridLayout();
    QLabel* idLabel = new QLabel("id: " + QString::number(id));
    idLabel->setStyleSheet("background-color: #f0f0f0; padding: 8px; border-radius: 4px; border: 1px solid #ddd;");
    layout->addWidget(idLabel, 0, 0, 1, 2, Qt::AlignCenter);
    QLabel* measurementLabel = new QLabel(QString::fromStdString(text));
    measurementLabel->setStyleSheet("background-color: #f0f0f0; padding: 8px; border-radius: 4px; border: 1px solid #ddd;");
    layout->addWidget(measurementLabel, 1, 0);
    QLabel* measurementValue = new QLabel(QString::fromStdString(text));
    measurementValue->setStyleSheet("background-color: #f0f0f0; padding: 8px; border-radius: 4px; border: 1px solid #ddd;");
    layout->addWidget(measurementValue, 1, 1);
    for (int i = 0; i < btns.size(); i++)
        layout->addWidget(btns[i], 2, i);

    container->setLayout(layout);

    deviceContainers.push_back(container);
    
    deviceLayout->insertWidget(deviceLayout->count() - 1, container);
}

QWidget* SmartHomeDashboard::getDevice(int index)
{
    if (index >= 0 && index < static_cast<int>(deviceContainers.size()))
        return deviceContainers[index];
    return nullptr;
}

void SmartHomeDashboard::setDeviceState(int index, const std::string& text)
{
    auto container = getDevice(index);
    if (!container)
        return;
    auto layout = container->findChild<QGridLayout*>();
    if (!layout || layout->count() <= static_cast<int>(DeviceLayoutItem::MESAURMENT_VALUE))
        return;
    auto label = qobject_cast<QLabel*>(layout->
        itemAt(static_cast<int>(DeviceLayoutItem::MESAURMENT_VALUE))->widget());
    if (label)
        label->setText(QString::fromStdString(text));
}