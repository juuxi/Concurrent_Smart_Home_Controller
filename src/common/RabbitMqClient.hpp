#include <utils.h>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>

#include <iostream>
#include <cstring>

struct RabbitMqClient
{
    static void listen(char const* hostname, int port, char const* exchange, char const* bindingkey);
    static void sendData(char const* hostname, int port, char const* exchange, char const* routingkey, char const* messagebody);
};