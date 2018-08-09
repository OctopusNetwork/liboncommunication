#ifndef __ONC_BASE_SOCKET____H__
#define __ONC_BASE_SOCKET____H__

#include <sys/socket.h>

#include "ocnet_socket.h"
#include "ocnet_iport.h"

#ifdef __cplusplus
extern "C" {
#endif

void   *base_socket_new(ocnet_socktype_t type,
            ocnet_ip_t ip, ocnet_port_t port, int events);
void    base_socket_del(void *sock);
int     base_socket_event_enroll(void *sock, void *evgrp);
int     base_socket_event_wait(void *sock, void *lfds, int timeout);
int     base_socket_event_del(void *sock, void *evgrp);
int     base_socket_readable(void *sock, void *lfds);
int     base_socket_send(void *sock, ocnet_conn_t conn, char *buf, int len);
int     base_socket_recv(void *sock, ocnet_conn_t conn, char *buf, int len);
int     base_socket_sendto(void *sock, char *buf, int len,
            ocnet_ip_t ip, ocnet_port_t port);
int     base_socket_recvfrom(void *sock, char *buf, int len,
            ocnet_ip_t *ip, ocnet_port_t *port);
int     base_socket_listen(void *sock, int backlog);
ocnet_conn_t      base_socket_accept(void *sock, ocnet_ip_t *ip, ocnet_port_t *port);
ocnet_conn_t      base_socket_connect(void *sock, ocnet_ip_t ip, ocnet_port_t port);
int               base_socket_close_connection(void *sock, ocnet_conn_t conn);
ocnet_ipvx_t      base_socket_get_ipvx(void *sock);
ocnet_ip_t        base_socket_get_ip(void *sock);
ocnet_port_t      base_socket_get_port(void *sock);

#ifdef __cplusplus
}
#endif

#endif
