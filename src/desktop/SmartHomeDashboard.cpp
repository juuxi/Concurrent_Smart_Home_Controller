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
    typeDependentUIHandlers.push_back([this](QGridLayout* layout, size_t device_id) {
        std::string text = "Brightness:";
        QLabel* measurementLabel = new QLabel(QString::fromStdString(text));
        measurementLabel->setObjectName("measuringLabel");
        layout->addWidget(measurementLabel, 1, 0);

        std::vector<QPushButton*> btns;
        auto off_btn = new QPushButton("Off");
        btns.push_back(off_btn);
        connect(off_btn, &QPushButton::clicked, this, [this, device_id]() {
            this->handleOffButton(device_id);
        });

        auto on_btn = new QPushButton("On");
        btns.push_back(on_btn);
        connect(on_btn, &QPushButton::clicked, this, [this, device_id]() {
            this->handleOnButton(device_id);
        });

        for (int i = 0; i < btns.size(); i++)
            layout->addWidget(btns[i], 2, i);
    });

    typeDependentUIHandlers.push_back([this](QGridLayout* layout, size_t device_id) {
        std::string text = "Temperature:";
        QLabel* measurementLabel = new QLabel(QString::fromStdString(text));
        measurementLabel->setObjectName("measuringLabel");
        layout->addWidget(measurementLabel, 1, 0);

        std::vector<QPushButton*> btns;
        auto cool_btn = new QPushButton("Cool");
        btns.push_back(cool_btn);
        connect(cool_btn, &QPushButton::clicked, this, [this, device_id]() {
            this->handleCoolButton(device_id);
        });
        auto warm_btn = new QPushButton("Warm");
        btns.push_back(warm_btn);
        connect(warm_btn, &QPushButton::clicked, this, [this, device_id]() {
            this->handleWarmButton(device_id);
        });

        for (int i = 0; i < btns.size(); i++)
            layout->addWidget(btns[i], 2, i);
    });
}

void SmartHomeDashboard::addDevice(size_t id, DeviceType type)
{
    QWidget* container = new QWidget();
    container->setObjectName("deviceCard");
    QGridLayout* layout = new QGridLayout();
    QLabel* idLabel = new QLabel("id: " + QString::number(id));
    idLabel->setObjectName("deviceId");
    layout->addWidget(idLabel, 0, 0, 1, 2, Qt::AlignCenter);
    QLabel* measurementValue = new QLabel("None");
    measurementValue->setObjectName("measuringValue");
    layout->addWidget(measurementValue, 1, 1);
    typeDependentUIHandlers[static_cast<int>(type)](layout, id);

    container->setLayout(layout);

    deviceContainers[id] = container;
    
    deviceLayout->insertWidget(deviceLayout->count() - 1, container);
}

QWidget* SmartHomeDashboard::getDevice(size_t id)
{
    if (id >= deviceContainers.begin()->first && id <= deviceContainers.rbegin()->first)
        return deviceContainers[id];
    return nullptr;
}

void SmartHomeDashboard::setDeviceState(size_t id, const std::string& text)
{
    auto label = getValueLabel(id);
    if (label)
        label->setText(QString::fromStdString(text));
}

void SmartHomeDashboard::setServer(std::shared_ptr<DeviceServer> newServer)
{
    server = newServer;
}

void SmartHomeDashboard::handleOnButton(size_t id)
{
    if (server)
        server->transmitData(id, "full");
}

void SmartHomeDashboard::handleOffButton(size_t id)
{
    if (server)
        server->transmitData(id, "lights off");
}

QLabel* SmartHomeDashboard::getValueLabel(size_t id)
{
    auto container = getDevice(id);
    if (!container)
        return nullptr;
    auto layout = container->findChild<QGridLayout*>();
    if (!layout || !layout->itemAtPosition(1, 1))
        return nullptr;
    auto label = qobject_cast<QLabel*>(layout->itemAtPosition(1, 1)->widget());
    return label;
}

int SmartHomeDashboard::getCurrentDeviceTemp(size_t id)
{
    auto label = getValueLabel(id);
    int currTemp = std::stoi(label->text().toStdString());
    return currTemp;
}

void SmartHomeDashboard::handleCoolButton(size_t id)
{
    int currTemp = getCurrentDeviceTemp(id);
    if (server)
        server->transmitData(id, std::to_string(currTemp - 1));
}

void SmartHomeDashboard::handleWarmButton(size_t id)
{
    int currTemp = getCurrentDeviceTemp(id);
    if (server)
        server->transmitData(id, std::to_string(currTemp + 1));
}