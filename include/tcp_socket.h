#ifndef __ONC_TCP_SOCKET____H__
#define __ONC_TCP_SOCKET____H__

#include <sys/select.h>

#include "on_socket.h"
#include "on_iport.h"

#ifdef __cplusplus
extern "C" {
#endif

void   *tcp_socket_new(onc_ip_t ip, onc_port_t port,
            int backlog, int events);
void    tcp_socket_del(void *sock);
int     tcp_socket_event_enroll(void *sock, void *evgrp);
int     tcp_socket_event_del(void *sock, void *evgrp);
int     tcp_socket_listen(void *sock);
onc_ipvx_e_t    tcp_socket_get_ipvx(void *sock);
onc_ip_t        tcp_socket_get_ip(void *sock);
onc_port_t      tcp_socket_get_port(void *sock);

#ifdef __cplusplus
}
#endif

#endif
