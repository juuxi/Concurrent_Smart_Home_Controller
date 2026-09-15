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

    setupTypeDependentUIHandlers();
}

void SmartHomeDashboard::setupTypeDependentUIHandlers()
{
    typeDependentUIHandlers.push_back([](QGridLayout* layout) {
        std::string text = "Brightness:";
        QLabel* measurementLabel = new QLabel(QString::fromStdString(text));
        measurementLabel->setStyleSheet("background-color: #f0f0f0; padding: 8px; border-radius: 4px; border: 1px solid #ddd;");
        layout->addWidget(measurementLabel, 1, 0);

        std::vector<QPushButton*> btns;
        auto off_btn = new QPushButton("Off");
        btns.push_back(off_btn);
        auto on_btn = new QPushButton("On");
        btns.push_back(on_btn);
        for (int i = 0; i < btns.size(); i++)
            layout->addWidget(btns[i], 2, i);
    });

    typeDependentUIHandlers.push_back([](QGridLayout* layout) {
        std::string text = "Temperature:";
        QLabel* measurementLabel = new QLabel(QString::fromStdString(text));
        measurementLabel->setStyleSheet("background-color: #f0f0f0; padding: 8px; border-radius: 4px; border: 1px solid #ddd;");
        layout->addWidget(measurementLabel, 1, 0);

        std::vector<QPushButton*> btns;
        auto cool_btn = new QPushButton("Cool");
        btns.push_back(cool_btn);
        auto warm_btn = new QPushButton("Warm");
        btns.push_back(warm_btn);
        for (int i = 0; i < btns.size(); i++)
            layout->addWidget(btns[i], 2, i);
    });
}

void SmartHomeDashboard::addDevice(int id, DeviceType type)
{
    QWidget* container = new QWidget();
    container->setStyleSheet("border-radius: 4px; border: 1px solid #ccc");
    QGridLayout* layout = new QGridLayout();
    QLabel* idLabel = new QLabel("id: " + QString::number(id));
    idLabel->setStyleSheet("background-color: #f0f0f0; padding: 8px; border-radius: 4px; border: 1px solid #ddd;");
    layout->addWidget(idLabel, 0, 0, 1, 2, Qt::AlignCenter);
    QLabel* measurementValue = new QLabel("None");
    measurementValue->setStyleSheet("background-color: #f0f0f0; padding: 8px; border-radius: 4px; border: 1px solid #ddd;");
    layout->addWidget(measurementValue, 1, 1);
    typeDependentUIHandlers[static_cast<int>(type)](layout);

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
    if (!layout || !layout->itemAtPosition(1, 1))
        return;
    auto label = qobject_cast<QLabel*>(layout->itemAtPosition(1, 1)->widget());
    if (label)
        label->setText(QString::fromStdString(text));
}