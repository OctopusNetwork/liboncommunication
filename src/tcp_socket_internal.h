#ifndef __ONC_TCP_SOCKET_INTERNAL____H__
#define __ONC_TCP_SOCKET_INTERNAL____H__

#include "on_socket.h"

#include "tcp_socket.h"

#ifdef __cplusplus
extern "C" {
#endif

int tcp_socket_send(void *sock, onc_conn_t conn, char *buf, int len);
int tcp_socket_recv(void *sock, onc_conn_t conn, char *buf, int len);
int tcp_socket_close_connection(void *sock, onc_conn_t conn);
int tcp_socket_event_wait(void *sock, void *lfds, int timeout);
int tcp_socket_acceptable(void *sock, void *lfds);
onc_conn_t tcp_socket_connect(void *sock, onc_ip_t ip, onc_port_t port);
onc_conn_t tcp_socket_accept(void *sock, onc_ip_t *ip, onc_port_t *port);

#ifdef __cplusplus
}
#endif

#endif
