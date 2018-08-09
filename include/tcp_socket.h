#ifndef __ONC_TCP_SOCKET____H__
#define __ONC_TCP_SOCKET____H__

#include <sys/select.h>

#include "ocnet_socket.h"
#include "ocnet_iport.h"

#ifdef __cplusplus
extern "C" {
#endif

void   *tcp_socket_new(ocnet_ip_t ip, ocnet_port_t port,
            int backlog, int events);
void    tcp_socket_del(void *sock);
int     tcp_socket_event_enroll(void *sock, void *evgrp);
int     tcp_socket_event_del(void *sock, void *evgrp);
int     tcp_socket_listen(void *sock);
ocnet_ipvx_t      tcp_socket_get_ipvx(void *sock);
ocnet_ip_t        tcp_socket_get_ip(void *sock);
ocnet_port_t      tcp_socket_get_port(void *sock);

#ifdef __cplusplus
}
#endif

#endif
