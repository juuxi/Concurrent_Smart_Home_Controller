// utils.h
// Copyright 2007 - 2021, Alan Antonuk and the rabbitmq-c contributors.
// SPDX-License-Identifier: MIT

#ifndef librabbitmq_examples_utils_h
#define librabbitmq_examples_utils_h

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/framing.h>

#ifdef __cplusplus
extern "C" {
#endif

  void die(const char *fmt, ...);
  void die_on_error(int x, const char *context);
  void die_on_amqp_error(amqp_rpc_reply_t x, const char *context);
  void amqp_dump(void const *buffer, size_t len);

  uint64_t now_microseconds(void);
  void microsleep(int usec);

#ifdef __cplusplus
}
#endif

#endif /* librabbitmq_examples_utils_h */