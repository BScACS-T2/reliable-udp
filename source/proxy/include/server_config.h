#ifndef CLIENT_SERVER_CONFIG_H
#define CLIENT_SERVER_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/un.h>
#include "fsm.h"

int         socket_create(int domain, int type, int protocol, struct fsm_error *err);
int         start_listening(int sockfd, int backlog, struct fsm_error *err);
int         socket_accept_connection(int sockfd, struct fsm_error *err);
int         socket_close(int sockfd, struct fsm_error *err);
int         convert_address(const char *address, struct sockaddr_storage *addr,
                            in_port_t port, struct fsm_error *err);
int         socket_bind(int sockfd, struct sockaddr_storage *addr, in_port_t port, struct fsm_error *err);
int         send_stats_gui(int sockfd, uint8_t stat);


#endif //CLIENT_SERVER_CONFIG_H
