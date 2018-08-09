#ifndef __KKT_TCP_CONNECTION____H__
#define __KKT_TCP_CONNECTION____H__

#include "ocnet_iport.h"

#ifdef __cplusplus
extern "C" {
#endif

void *tcp_connection_accept(void *sock, int event_wait,
            void *lfds, int millseconds);
void *tcp_connection_connect(void *sock, ocnet_ip_t ip, ocnet_port_t port);
void  tcp_connection_del(void *tcp_conn);
ocnet_ip_t tcp_connection_get_ip(void *tcp_conn);
ocnet_ip_t tcp_connection_get_port(void *tcp_conn);

int   tcp_connection_event_enroll(void *tcp_conn, void *evgrp);
int   tcp_connection_event_del(void *tcp_conn, void *evgrp);
int   tcp_connection_event_wait(void *tcp_conn, void *lfds, int timeout);
int   tcp_connection_read(void *tcp_conn, char *buf, int size);
int   tcp_connection_readable(void *tcp_conn, void *lfds);
int   tcp_connection_write(void *tcp_conn, char *buf, int size);
int   tcp_connection_writable(void *tcp_conn, void *lfds);
int   tcp_connection_error(void *tcp_conn, void *lfds);
void  tcp_connection_wait_writable(void *tcp_conn);
void  tcp_connection_dontwait_writable(void *tcp_conn);

#ifdef __cplusplus
}
#endif

#endif
