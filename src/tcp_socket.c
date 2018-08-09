#include "ocnet_malloc.h"
#include "ocnet_socket.h"
#include "ocnet_iport.h"

#include "onevent.h"
#include "onevgrp.h"

#include "base_socket.h"

#include "tcp_socket_internal.h"

typedef struct {
    void    *base_sock;
    int      backlog;
} tcp_socket_t;

void *tcp_socket_new(ocnet_ip_t ip, ocnet_port_t port,
        int backlog, int events)
{
    void          *base_sock = NULL;
    tcp_socket_t  *tcp_sock = NULL;

    base_sock = base_socket_new(OCNET_SOCK_TCP, ip, port, events);
    if (NULL == base_sock) {
        return NULL;
    }

    tcp_sock = (tcp_socket_t *)ocnet_malloc(sizeof(tcp_socket_t));
    if (NULL == tcp_sock) {
        base_socket_del(base_sock);
        return NULL;
    }

    tcp_sock->base_sock = base_sock;
    tcp_sock->backlog = backlog;
    return tcp_sock;
}

void tcp_socket_del(void *sock)
{
    tcp_socket_t *tcp_sock = (tcp_socket_t *)sock;
    base_socket_del(tcp_sock->base_sock);
    ocnet_free(tcp_sock);
}

int tcp_socket_listen(void *sock)
{
    tcp_socket_t *tcp_sock = (tcp_socket_t *)sock;
    return base_socket_listen(tcp_sock->base_sock, tcp_sock->backlog);
}

int tcp_socket_event_enroll(void *sock, void *evgrp)
{
    tcp_socket_t *tcp_sock = (tcp_socket_t *)sock;
    return base_socket_event_enroll(tcp_sock->base_sock, evgrp);
}

int tcp_socket_event_del(void *sock, void *evgrp)
{
    tcp_socket_t *tcp_sock = (tcp_socket_t *)sock;
    return base_socket_event_del(tcp_sock->base_sock, evgrp);
}

int tcp_socket_send(void *sock, ocnet_conn_t conn, char *buf, int len)
{
    tcp_socket_t *tcp_sock = (tcp_socket_t *)sock;
    return base_socket_send(tcp_sock->base_sock, conn, buf, len);
}

int tcp_socket_recv(void *sock, ocnet_conn_t conn, char *buf, int len)
{
    tcp_socket_t *tcp_sock = (tcp_socket_t *)sock;
    return base_socket_recv(tcp_sock->base_sock, conn, buf, len);
}

ocnet_conn_t tcp_socket_connect(void *sock, ocnet_ip_t ip, ocnet_port_t port)
{
    tcp_socket_t *tcp_sock = (tcp_socket_t *)sock;
    return base_socket_connect(tcp_sock->base_sock, ip, port);
}

int tcp_socket_event_wait(void *sock, void *lfds, int timeout)
{
    tcp_socket_t *tcp_sock = (tcp_socket_t *)sock;
    return base_socket_event_wait(tcp_sock->base_sock, lfds, timeout);
}

int tcp_socket_acceptable(void *sock, void *lfds)
{
    tcp_socket_t *tcp_sock = (tcp_socket_t *)sock;
    return base_socket_readable(tcp_sock->base_sock, lfds);
}

ocnet_conn_t tcp_socket_accept(void *sock, ocnet_ip_t *ip, ocnet_port_t *port)
{
    tcp_socket_t *tcp_sock = (tcp_socket_t *)sock;
    return base_socket_accept(tcp_sock->base_sock, ip, port);
}

int tcp_socket_close_connection(void *sock, ocnet_conn_t conn)
{
    tcp_socket_t *tcp_sock = (tcp_socket_t *)sock;
    return base_socket_close_connection(tcp_sock->base_sock, conn);
}
ocnet_ipvx_t tcp_socket_get_ipvx(void *sock)
{
    tcp_socket_t *tcp_sock = (tcp_socket_t *)sock;
    return base_socket_get_ipvx(tcp_sock->base_sock);
}

ocnet_ip_t tcp_socket_get_ip(void *sock)
{
    tcp_socket_t *tcp_sock = (tcp_socket_t *)sock;
    return base_socket_get_ip(tcp_sock->base_sock);
}

ocnet_port_t tcp_socket_get_port(void *sock)
{
    tcp_socket_t *tcp_sock = (tcp_socket_t *)sock;
    return base_socket_get_port(tcp_sock->base_sock);
}
