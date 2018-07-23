#ifndef __ONC_BASE_SOCKET____H__
#define __ONC_BASE_SOCKET____H__

#include <sys/socket.h>

#include "on_socket.h"
#include "on_iport.h"

#ifdef __cplusplus
extern "C" {
#endif

void   *base_socket_new(onc_socktype_e_t type,
            onc_ip_t ip, onc_port_t port, int events);
void    base_socket_del(void *sock);
int     base_socket_event_enroll(void *sock, void *evgrp);
int     base_socket_event_wait(void *sock, void *lfds, int timeout);
int     base_socket_event_del(void *sock, void *evgrp);
int     base_socket_readable(void *sock, void *lfds);
int     base_socket_send(void *sock, onc_conn_t conn, char *buf, int len);
int     base_socket_recv(void *sock, onc_conn_t conn, char *buf, int len);
int     base_socket_sendto(void *sock, char *buf, int len,
            onc_ip_t ip, onc_port_t port);
int     base_socket_recvfrom(void *sock, char *buf, int len,
            onc_ip_t *ip, onc_port_t *port);
int     base_socket_listen(void *sock, int backlog);
onc_conn_t      base_socket_accept(void *sock, onc_ip_t *ip, onc_port_t *port);
onc_conn_t      base_socket_connect(void *sock, onc_ip_t ip, onc_port_t port);
int             base_socket_close_connection(void *sock, onc_conn_t conn);
onc_ipvx_e_t    base_socket_get_ipvx(void *sock);
onc_ip_t        base_socket_get_ip(void *sock);
onc_port_t      base_socket_get_port(void *sock);

#ifdef __cplusplus
}
#endif

#endif
