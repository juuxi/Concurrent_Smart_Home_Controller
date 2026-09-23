#include <SmartHomeDashboard.hpp>

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

    connect(this, &SmartHomeDashboard::deviceReceived, this, &SmartHomeDashboard::addDevice);

    parseConfigFile("dashboard_config.ini");
    setupListenThread();
    setupPollQueueThread();
}

void SmartHomeDashboard::setupListenThread()
{
    listenThread = std::jthread(
        [config = config, messageQueue = std::ref(messageQueue)]()
        {
            try
            {
                RabbitMqClient::listen(config.at("SERVER_HOST").c_str(), std::stoi(config.at("SERVER_PORT")),
                                       config.at("SERVER_EXCHANGE").c_str(), config.at("SERVER_ROUTING_KEY").c_str(), messageQueue);
            }
            catch (const std::exception& e)
            {
                std::cerr << "Listening thread failed: " << e.what() << '\n';
            }
        }
    );
}

void SmartHomeDashboard::setupPollQueueThread()
{
    pollQueueThread = std::jthread(
        [this]()
        {
            while (true)
            {
                if (!messageQueue.empty())
                {
                    auto message = messageQueue.front();
                    messageQueue.pop();

                    Messages::UpdateDataMessage updateMessage;
                    if (!updateMessage.ParseFromString(message))
                    {
                        std::cerr << "Failed to parse message from server: " << message << std::endl;
                        continue;
                    }

                    if (updateMessage.has_new_device_data())
                    {
                        int id = updateMessage.new_device_data().id();
                        int type = updateMessage.new_device_data().type() - 1;  // Adjusting from 1-based enum in protobuf

                        emit deviceReceived(id, type);
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    );
}

void SmartHomeDashboard::parseConfigFile(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Failed to open config file at: " << filename << "\n";
        return;
    }

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) {
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);
            
            config[key] = value;
        }
    }
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

        auto half_lights_btn = new QPushButton("Half");
        btns.push_back(half_lights_btn);
        connect(half_lights_btn, &QPushButton::clicked, this, [this, device_id]() {
            this->handleHalfLightsButton(device_id);
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

void SmartHomeDashboard::addDevice(int id, int type)
{
    QWidget* container = new QWidget();
    container->setObjectName("deviceCard");
    QGridLayout* layout = new QGridLayout();
    QLabel* idLabel = new QLabel("id: " + QString::number(id));
    idLabel->setObjectName("deviceId");
    QLabel* measurementValue = new QLabel("None");
    measurementValue->setObjectName("measuringValue");
    typeDependentUIHandlers[type](layout, id);

    layout->addWidget(idLabel, 0, 0, 1, layout->columnCount(), Qt::AlignCenter);
    layout->addWidget(measurementValue, 1, 1, 1, layout->columnCount() - 1);

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
    ;
    // if (server)
    //    server->transmitData(id, "full");
}

void SmartHomeDashboard::handleHalfLightsButton(size_t id)
{
    ;
    // if (server)
    //    server->transmitData(id, "50%");
}

void SmartHomeDashboard::handleOffButton(size_t id)
{
    ;
    // if (server)
    //    server->transmitData(id, "lights off");
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
    //if (server)
    //    server->transmitData(id, std::to_string(currTemp - 1));
}

void SmartHomeDashboard::handleWarmButton(size_t id)
{
    int currTemp = getCurrentDeviceTemp(id);
    //if (server)
    //    server->transmitData(id, std::to_string(currTemp + 1));
}