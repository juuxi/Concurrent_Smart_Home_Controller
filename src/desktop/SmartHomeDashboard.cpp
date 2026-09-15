#include <SmartHomeDashboard.hpp>
#include <DeviceServer.hpp>

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
    typeDependentUIHandlers.push_back([this](QGridLayout* layout, int device_index) {
        std::string text = "Brightness:";
        QLabel* measurementLabel = new QLabel(QString::fromStdString(text));
        measurementLabel->setStyleSheet("background-color: #f0f0f0; padding: 8px; border-radius: 4px; border: 1px solid #ddd;");
        layout->addWidget(measurementLabel, 1, 0);

        std::vector<QPushButton*> btns;
        auto off_btn = new QPushButton("Off");
        btns.push_back(off_btn);
        connect(off_btn, &QPushButton::clicked, this, [this, device_index]() {
            this->handleOffButton(device_index);
        });

        auto on_btn = new QPushButton("On");
        btns.push_back(on_btn);
        connect(on_btn, &QPushButton::clicked, this, [this, device_index]() {
            this->handleOnButton(device_index);
        });

        for (int i = 0; i < btns.size(); i++)
            layout->addWidget(btns[i], 2, i);
    });

    typeDependentUIHandlers.push_back([this](QGridLayout* layout, int device_index) {
        std::string text = "Temperature:";
        QLabel* measurementLabel = new QLabel(QString::fromStdString(text));
        measurementLabel->setStyleSheet("background-color: #f0f0f0; padding: 8px; border-radius: 4px; border: 1px solid #ddd;");
        layout->addWidget(measurementLabel, 1, 0);

        // !! REWRITE !! warm and cool logics are essentially the same 
        std::vector<QPushButton*> btns;
        auto cool_btn = new QPushButton("Cool");
        btns.push_back(cool_btn);
        connect(cool_btn, &QPushButton::clicked, this, [this, device_index]() {
            auto container = getDevice(device_index);
            if (!container)
                return;
            auto layout = container->findChild<QGridLayout*>();
            if (!layout || !layout->itemAtPosition(1, 1))
                return;
            auto label = qobject_cast<QLabel*>(layout->itemAtPosition(1, 1)->widget());
            int currTemp = std::stoi(label->text().toStdString());
            this->handleCoolButton(device_index, currTemp);
        });
        auto warm_btn = new QPushButton("Warm");
        btns.push_back(warm_btn);
        connect(warm_btn, &QPushButton::clicked, this, [this, device_index]() {
            auto container = getDevice(device_index);
            if (!container)
                return;
            auto layout = container->findChild<QGridLayout*>();
            if (!layout || !layout->itemAtPosition(1, 1))
                return;
            auto label = qobject_cast<QLabel*>(layout->itemAtPosition(1, 1)->widget());
            int currTemp = std::stoi(label->text().toStdString());
            this->handleWarmButton(device_index, currTemp);
        });
        // !! REWRITE !!

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
    typeDependentUIHandlers[static_cast<int>(type)](layout, id - 1);

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

void SmartHomeDashboard::setServer(std::shared_ptr<DeviceServer> newServer)
{
    server = newServer;
}

void SmartHomeDashboard::handleOnButton(int index)
{
    if (server)
        server->transmitData(index, "full");
}

void SmartHomeDashboard::handleOffButton(int index)
{
    if (server)
        server->transmitData(index, "lights off");
}

void SmartHomeDashboard::handleCoolButton(int index, int currTemp)
{
    if (server)
        server->transmitData(index, std::to_string(currTemp - 1));
}

void SmartHomeDashboard::handleWarmButton(int index, int currTemp)
{
    if (server)
        server->transmitData(index, std::to_string(currTemp + 1));
}