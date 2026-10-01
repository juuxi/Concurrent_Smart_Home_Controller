#include <SmartHomeDashboard.hpp>

SmartHomeDashboard::SmartHomeDashboard(QWidget* parent)
    : QWidget(parent)
{
    setWindowState(Qt::WindowMaximized);
    setWindowTitle("Smart Home Dashboard");

    setupUI();
    setupTypeDependentUIHandlers();

    connect(this, &SmartHomeDashboard::deviceReceived, this, &SmartHomeDashboard::addDevice);
    connect(this, &SmartHomeDashboard::deviceStateReceived, this, &SmartHomeDashboard::setDeviceState);
    connect(addDeviceBtn, &QPushButton::clicked, this, &SmartHomeDashboard::showAddDialog);

    parseConfigFile("dashboard_config.ini");
    setupListenThread();
    setupPollQueueThread();
}

void SmartHomeDashboard::setupUI()
{
    labelContainer = new QWidget(this);
    deviceLayout = new QHBoxLayout(labelContainer);
    deviceLayout->setContentsMargins(30, 30, 30, 0);
    deviceLayout->setSpacing(10);
    deviceLayout->addStretch();

    addDeviceBtn = new QPushButton(this);
    addDeviceBtn->setText("");
    addDeviceBtn->setIcon(createGreenPlusIcon());
    addDeviceBtn->setIconSize(QSize(32, 32));
    addDeviceBtn->setFixedSize(48, 48);
    addDeviceBtn->setToolTip("Add");

    addDeviceBtn->setStyleSheet(R"(
        QPushButton {
            border: none;
            background: transparent;
        }
        QPushButton:hover {
            background-color: rgba(32, 180, 90, 35);
            border-radius: 24px;
        }
        QPushButton:pressed {
            background-color: rgba(32, 180, 90, 70);
            border-radius: 24px;
        }
    )");

    deviceLayout->addWidget(addDeviceBtn, 0, Qt::AlignTop);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(labelContainer, 0, Qt::AlignTop);
    layout->addStretch();
    setLayout(layout);
}

QIcon SmartHomeDashboard::createGreenPlusIcon(int size)
{
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(QColor("#20B45A"));  // Green
    pen.setWidth(4);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);

    const int center = size / 2;
    const int arm = size / 4;

    painter.drawLine(center - arm, center, center + arm, center);
    painter.drawLine(center, center - arm, center, center + arm);

    return QIcon(pixmap);
}

void SmartHomeDashboard::setupListenThread()
{
    listenThread = std::jthread(
        [config = config, messageQueue = std::ref(messageQueue)]()
        {
            try
            {
                RabbitMqClient::listen(config.at("FROM_SERVER_HOST").c_str(), std::stoi(config.at("FROM_SERVER_PORT")),
                                       config.at("FROM_SERVER_EXCHANGE").c_str(), config.at("FROM_SERVER_ROUTING_KEY").c_str(), 
                                       config.at("FROM_SERVER_QUEUE").c_str(), messageQueue);
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
                        int type = updateMessage.new_device_data().type();

                        emit deviceReceived(id, type);
                    }

                    if (updateMessage.has_device_measurement())
                    {
                        int id = updateMessage.device_measurement().id();
                        if (updateMessage.device_measurement().has_light_device_measurement())
                        {
                            const std::string brightness = updateMessage.device_measurement().light_device_measurement().brightness();
                            emit deviceStateReceived(id, QString::fromStdString(brightness));
                        }
                        else if (updateMessage.device_measurement().has_temperature_device_measurement())
                        {
                            const std::string temperature = std::to_string(updateMessage.device_measurement().temperature_device_measurement().temperature());
                            emit deviceStateReceived(id, QString::fromStdString(temperature));
                        }
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
    typeDependentUIHandlers.push_back([this](QGridLayout* layout, size_t deviceId) {
        std::string text = "Brightness:";
        QLabel* measurementLabel = new QLabel(QString::fromStdString(text));
        measurementLabel->setObjectName("measuringLabel");
        layout->addWidget(measurementLabel, 1, 0);

        std::vector<QPushButton*> btns;
        auto offBtn = new QPushButton("Off");
        btns.push_back(offBtn);
        connect(offBtn, &QPushButton::clicked, this, [this, deviceId]() {
            this->handleOffButton(deviceId);
        });

        auto halfLightsBtn = new QPushButton("Half");
        btns.push_back(halfLightsBtn);
        connect(halfLightsBtn, &QPushButton::clicked, this, [this, deviceId]() {
            this->handleHalfLightsButton(deviceId);
        });

        auto onBtn = new QPushButton("On");
        btns.push_back(onBtn);
        connect(onBtn, &QPushButton::clicked, this, [this, deviceId]() {
            this->handleOnButton(deviceId);
        });

        for (int i = 0; i < btns.size(); i++)
            layout->addWidget(btns[i], 2, i);
    });

    typeDependentUIHandlers.push_back([this](QGridLayout* layout, size_t deviceId) {
        std::string text = "Temperature:";
        QLabel* measurementLabel = new QLabel(QString::fromStdString(text));
        measurementLabel->setObjectName("measuringLabel");
        layout->addWidget(measurementLabel, 1, 0);

        std::vector<QPushButton*> btns;
        auto coolBtn = new QPushButton("Cool");
        btns.push_back(coolBtn);
        connect(coolBtn, &QPushButton::clicked, this, [this, deviceId]() {
            this->handleCoolButton(deviceId);
        });
        auto warmBtn = new QPushButton("Warm");
        btns.push_back(warmBtn);
        connect(warmBtn, &QPushButton::clicked, this, [this, deviceId]() {
            this->handleWarmButton(deviceId);
        });

        for (int i = 0; i < btns.size(); i++)
            layout->addWidget(btns[i], 2, i);
    });
}

void SmartHomeDashboard::sendMessageToServer(const Messages::ChangeDataMessage& message)
{
    size_t size = message.ByteSizeLong();
    char *buffer = new char[size];
    message.SerializeToArray(buffer, size);

    RabbitMqClient::sendData(config.at("TO_SERVER_HOST").c_str(), std::stoi(config.at("TO_SERVER_PORT")),
                             config.at("TO_SERVER_EXCHANGE").c_str(), config.at("TO_SERVER_ROUTING_KEY").c_str(),
                             buffer, size);
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
    
    deviceLayout->insertWidget(deviceLayout->count() - 2, container);
}

QWidget* SmartHomeDashboard::getDevice(size_t id)
{
    if (id >= deviceContainers.begin()->first && id <= deviceContainers.rbegin()->first)
        return deviceContainers[id];
    return nullptr;
}

void SmartHomeDashboard::setDeviceState(int id, const QString text)
{
    auto label = getValueLabel(id);
    if (label)
        label->setText(text);
}

void SmartHomeDashboard::setServer(std::shared_ptr<DeviceServer> newServer)
{
    server = newServer;
}

void SmartHomeDashboard::handleOnButton(size_t id)
{
    Messages::ChangeDataMessage message;
    message.set_id(id);
    message.mutable_change_light()->mutable_light_on();

    sendMessageToServer(message);
}

void SmartHomeDashboard::handleHalfLightsButton(size_t id)
{
    Messages::ChangeDataMessage message;
    message.set_id(id);
    message.mutable_change_light()->mutable_light_half();

    sendMessageToServer(message);
}

void SmartHomeDashboard::handleOffButton(size_t id)
{
    Messages::ChangeDataMessage message;
    message.set_id(id);
    message.mutable_change_light()->mutable_light_off();

    sendMessageToServer(message);
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

void SmartHomeDashboard::handleCoolButton(size_t id)
{
    Messages::ChangeDataMessage message;
    message.set_id(id);
    message.mutable_change_temperature()->mutable_decrease_temperature();

    sendMessageToServer(message);
}

void SmartHomeDashboard::handleWarmButton(size_t id)
{
    Messages::ChangeDataMessage message;
    message.set_id(id);
    message.mutable_change_temperature()->mutable_increase_temperature();

    sendMessageToServer(message);
}

void SmartHomeDashboard::showAddDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add Device");

    QVBoxLayout layout(&dialog);

    QRadioButton lightButton("Light");
    QRadioButton temperatureButton("Temperature");
    layout.addWidget(&lightButton);
    layout.addWidget(&temperatureButton);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout.addWidget(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        int type = lightButton.isChecked() ? 0 : 1;
        Messages::ChangeDataMessage message;
        message.set_id(0);
        message.mutable_new_device_type()->set_type(Messages::NewDeviceType_DeviceType(static_cast<int>(type)));

        sendMessageToServer(message);
    }
}