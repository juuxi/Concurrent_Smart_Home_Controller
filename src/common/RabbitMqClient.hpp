#include <utils.h>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>

#include <iostream>
#include <cstring>
#include <queue>

struct RabbitMqClient
{
    static void listen(char const* hostname, int port, char const* exchange, char const* bindingkey, std::queue<std::string>& messageQueue);
    static void sendData(char const* hostname, int port, char const* exchange, char const* routingkey, char const* messagebody);
};