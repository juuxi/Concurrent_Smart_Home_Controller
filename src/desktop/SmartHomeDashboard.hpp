#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include "utils.h"

#include "Messages.pb.h"

#include <vector>
#include <map>
#include <thread>
#include <iostream>
#include <fstream>
#include <queue>

#include <RabbitMqClient.hpp>

class DeviceServer;

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
    void setDeviceState(size_t id, const std::string& text);
    
    QWidget* getDevice(size_t id);
    void setServer(std::shared_ptr<DeviceServer> newServer);

private:
    std::jthread listenThread;
    std::jthread pollQueueThread;
    std::queue<std::string> messageQueue;

    void setupListenThread();
    void setupPollQueueThread();
    void sendMessageToServer(const Messages::ChangeDataMessage& message);

    std::unordered_map<std::string, std::string> config;

    std::shared_ptr<DeviceServer> server;
    QWidget* labelContainer;
    QHBoxLayout* deviceLayout;
    std::map<size_t, QWidget*> deviceContainers;

    using HandleTypeDependentUI = std::function<void(QGridLayout*, size_t)>;
    std::vector<HandleTypeDependentUI> typeDependentUIHandlers;
    void setupTypeDependentUIHandlers();

    QLabel* getValueLabel(size_t id);

    void handleOffButton(size_t id);
    void handleHalfLightsButton(size_t id);
    void handleOnButton(size_t id);

    void handleCoolButton(size_t id);
    void handleWarmButton(size_t id);

    void parseConfigFile(const std::string& filename);

signals:
    void deviceReceived(int id, int type);
public slots:
    void addDevice(int id, int type);
};