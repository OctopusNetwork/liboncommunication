#include "on_malloc.h"
#include "on_socket.h"
#include "on_iport.h"

#include "onlfds.h"
#include "onevent.h"
#include "onevgrp.h"

#include "base_socket.h"

typedef struct {
    onc_socket_t    sock_fd;
    onc_ip_t        ip;
    onc_port_t      port;
    onc_ipvx_e_t    ip_version;
    void           *event;
} base_socket_s_t;

void *base_socket_new(onc_socktype_e_t type,
        onc_ip_t ip, onc_port_t port, int events)
{
    base_socket_s_t *base_sock = NULL;
    onc_socket_t     sock_fd = KKT_SOCKET_INVALID;
    int              base_sock_fd;

    sock_fd = onc_socket_open(type, ip, port);
    if (KKT_SOCKET_INVALID == sock_fd) {
        return NULL;
    }

    base_sock = (base_socket_s_t *)onc_malloc(sizeof(base_socket_s_t));
    if (NULL == base_sock) {
        onc_socket_close(sock_fd);
        return NULL;
    }

    base_sock_fd = onc_socket_get_fd(sock_fd);
    base_sock->event = onc_event_create(
            0, events, 0,
            base_sock_fd, base_sock_fd);
    if (NULL == base_sock->event) {
        onc_free(base_sock);
        onc_socket_close(sock_fd);
        return NULL;
    }

    base_sock->sock_fd = sock_fd;
    base_sock->ip = onc_socket_get_ip(sock_fd);
    base_sock->port = onc_socket_get_port(sock_fd);
    base_sock->ip_version = onc_iport_get_ipvx_byip(base_sock->ip);

    return base_sock;
}

void base_socket_del(void *sock)
{
    base_socket_s_t *base_sock = (base_socket_s_t *)sock;

    onc_event_destroy(base_sock->event);
    onc_socket_close(base_sock->sock_fd);
    onc_free(base_sock);
}

int base_socket_event_enroll(void *sock, void *evgrp)
{
    base_socket_s_t *base_sock = (base_socket_s_t *)sock;
    return onc_evgrp_event_add(evgrp, base_sock->event);
}

int base_socket_event_wait(void *sock, void *lfds, int timeout)
{
    base_socket_s_t *base_sock = (base_socket_s_t *)sock;
    return onc_event_wait(base_sock->event, lfds, timeout);
}

int base_socket_event_del(void *sock, void *evgrp)
{
    base_socket_s_t *base_sock = (base_socket_s_t *)sock;
    return onc_evgrp_event_del(evgrp, base_sock->event);
}

int base_socket_readable(void *sock, void *lfds)
{
    base_socket_s_t *base_sock = (base_socket_s_t *)sock;
    return onc_event_happen(base_sock->event, lfds, KKT_EVENT_READ);
}

int base_socket_sendto(void *sock, char *buf, int len,
        onc_ip_t ip, onc_port_t port)
{
    base_socket_s_t *base_sock = (base_socket_s_t *)sock;
    return onc_socket_sendto(base_sock->sock_fd, buf, len, ip, port);
}

int base_socket_recvfrom(void *sock, char *buf, int len,
        onc_ip_t *ip, onc_port_t *port)
{
    base_socket_s_t *base_sock = (base_socket_s_t *)sock;
    return onc_socket_recvfrom(base_sock->sock_fd, buf, len, ip, port);
}

int base_socket_send(void *sock, onc_conn_t conn, char *buf, int len)
{
    base_socket_s_t *base_sock = (base_socket_s_t *)sock;
    return onc_socket_send(base_sock->sock_fd, conn, buf, len);
}

int base_socket_recv(void *sock, onc_conn_t conn, char *buf, int len)
{
    base_socket_s_t *base_sock = (base_socket_s_t *)sock;
    return onc_socket_recv(base_sock->sock_fd, conn, buf, len);
}

int base_socket_listen(void *sock, int backlog)
{
    base_socket_s_t *base_sock = (base_socket_s_t *)sock;
    return onc_socket_listen(base_sock->sock_fd, backlog);
}

onc_conn_t base_socket_accept(void *sock, onc_ip_t *ip, onc_port_t *port)
{
    base_socket_s_t *base_sock = (base_socket_s_t *)sock;
    return onc_socket_accept(base_sock->sock_fd, ip, port);
}

onc_conn_t base_socket_connect(void *sock, onc_ip_t ip, onc_port_t port)
{
    base_socket_s_t *base_sock = (base_socket_s_t *)sock;
    return onc_socket_connect(base_sock->sock_fd, ip, port);
}

int base_socket_close_connection(void *sock, onc_conn_t conn)
{
    base_socket_s_t *base_sock = (base_socket_s_t *)sock;
    return onc_socket_close_connection(base_sock->sock_fd, conn);
}

onc_ipvx_e_t base_socket_get_ipvx(void *sock)
{
    base_socket_s_t *base_sock = (base_socket_s_t *)sock;
    return base_sock->ip_version;
}

onc_ip_t base_socket_get_ip(void *sock)
{
    base_socket_s_t *base_sock = (base_socket_s_t *)sock;
    return base_sock->ip;
}

onc_port_t base_socket_get_port(void *sock)
{
    base_socket_s_t *base_sock = (base_socket_s_t *)sock;
    return base_sock->port;
}
