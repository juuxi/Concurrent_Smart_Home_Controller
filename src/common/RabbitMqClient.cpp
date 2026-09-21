#include <RabbitMqClient.hpp>

void RabbitMqClient::listen(char const* hostname, int port, char const* exchange, char const* bindingkey)
{
    int status;
    amqp_socket_t *socket = NULL;
    amqp_connection_state_t conn;

    amqp_bytes_t queuename;

    conn = amqp_new_connection();

    socket = amqp_tcp_socket_new(conn);
    if (!socket) {
        die("creating TCP socket");
    }

    status = amqp_socket_open(socket, hostname, port);
    if (status) {
        die("opening TCP socket");
    }

    die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN,
                      "guest", "guest"),
                      "Logging in");
    amqp_channel_open(conn, 1);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");

    {
        amqp_queue_declare_ok_t *r = amqp_queue_declare(
            conn, 1, amqp_cstring_bytes("request_queue"), 0, 0, 0, 1, amqp_empty_table);
        die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring queue");
        queuename = amqp_bytes_malloc_dup(r->queue);
        if (queuename.bytes == NULL) {
            fprintf(stderr, "Out of memory while copying queue name");
        }
    }
    amqp_exchange_declare(conn, 1, amqp_cstring_bytes(exchange),
                          amqp_cstring_bytes("direct"), 0, 0, 0, 0, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring exchange");

    amqp_queue_bind(conn, 1, queuename, amqp_cstring_bytes(exchange),
                    amqp_cstring_bytes(bindingkey), amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Binding queue");

    amqp_basic_consume(conn, 1, queuename, amqp_empty_bytes, 0, 1, 0,
                       amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Consuming");

    {
        for(;;) {
            amqp_rpc_reply_t res;
            amqp_envelope_t envelope;
            amqp_maybe_release_buffers(conn);

            res = amqp_consume_message(conn, &envelope, NULL, 0);

            if (AMQP_RESPONSE_NORMAL != res.reply_type) {
                break;
            }

            printf("Delivery %u, exchange %.*s routingkey %.*s\n",
                    (unsigned)envelope.delivery_tag, (int)envelope.exchange.len,
                    (char *)envelope.exchange.bytes, (int)envelope.routing_key.len,
                    (char *)envelope.routing_key.bytes);

            if (envelope.message.properties._flags & AMQP_BASIC_CONTENT_TYPE_FLAG) {
                printf("Content-type: %.*s\n",
                        (int)envelope.message.properties.content_type.len,
                        (char *)envelope.message.properties.content_type.bytes);
            }
            printf("----\n");

            std::string message(static_cast<char*>(envelope.message.body.bytes), envelope.message.body.len);
            const auto id_begin = message.find("id:");
            const auto type_begin = message.find("type:");

            const auto id_start = id_begin + 3;
            const auto type_start = type_begin + 5;

            const std::size_t id = std::stoull(message.substr(id_start), nullptr);
            const int type = std::stoi(message.substr(type_start), nullptr);

            amqp_dump(envelope.message.body.bytes, envelope.message.body.len);

            amqp_destroy_envelope(&envelope);
        }
    }

    amqp_bytes_free(queuename);

    die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS),
                      "Closing channel");
    die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS),
                      "Closing connection");
    die_on_error(amqp_destroy_connection(conn), "Ending connection");
}

void RabbitMqClient::sendData(char const* hostname, int port, char const* exchange, char const* routingkey, char const* messagebody)
{
    int status;
    amqp_socket_t *socket = NULL;
    amqp_connection_state_t conn;

    conn = amqp_new_connection();

    socket = amqp_tcp_socket_new(conn);
    if (!socket) {
        die("creating TCP socket");
    }

    status = amqp_socket_open(socket, hostname, port);
    if (status != AMQP_STATUS_OK) {
        fprintf(stderr,
                "Cannot connect to %s:%d: status=%d (%s), errno=%d (%s)\n",
                hostname,
                port,
                status,
                amqp_error_string2(status),
                errno,
                strerror(errno));

        amqp_destroy_connection(conn);
        exit(EXIT_FAILURE);
    }

    die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN,
                                "guest", "guest"),
                    "Logging in");
    amqp_channel_open(conn, 1);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");

    amqp_exchange_declare(conn, 1, amqp_cstring_bytes(exchange),
                        amqp_cstring_bytes("direct"), 0, 0, 0, 0, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring exchange");

    {
    die_on_error(amqp_basic_publish(conn, 1, amqp_cstring_bytes(exchange),
                                    amqp_cstring_bytes(routingkey), 0, 0,
                                    NULL, amqp_cstring_bytes(messagebody)),
                    "Publishing");
    }

    die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS),
                    "Closing channel");
    die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS),
                    "Closing connection");
    die_on_error(amqp_destroy_connection(conn), "Ending connection");
}