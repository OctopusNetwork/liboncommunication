#include "ocnet_malloc.h"
#include "ocnet_socket.h"
#include "ocnet_iport.h"

#include "onlfds.h"
#include "onevent.h"
#include "onevgrp.h"

#include "base_socket.h"

typedef struct {
    ocnet_socket_t   *sock;
    ocnet_ip_t        ip;
    ocnet_port_t      port;
    ocnet_ipvx_t      ip_version;
    void             *event;
} base_socket_t;

void *base_socket_new(ocnet_socktype_t type,
        ocnet_ip_t ip, ocnet_port_t port, int events)
{
    base_socket_t *base_sock = NULL;
    ocnet_socket_t *sock = NULL;
    int base_sock_fd;

    sock = ocnet_socket_open(type, ip, port);
    if (NULL == sock) {
        return NULL;
    }

    base_sock = (base_socket_t *)ocnet_malloc(sizeof(base_socket_t));
    if (NULL == base_sock) {
        ocnet_socket_close(sock);
        return NULL;
    }

    base_sock_fd = ocnet_socket_get_fd(sock);
    base_sock->event = ocnet_event_create(
            0, events, 0,
            base_sock_fd, base_sock_fd);
    if (NULL == base_sock->event) {
        ocnet_free(base_sock);
        ocnet_socket_close(sock);
        return NULL;
    }

    base_sock->sock = sock;
    base_sock->ip = ocnet_socket_get_ip(sock);
    base_sock->port = ocnet_socket_get_port(sock);
    base_sock->ip_version = ocnet_iport_get_ipvx_byip(base_sock->ip);

    return base_sock;
}

void base_socket_del(void *sock)
{
    base_socket_t *base_sock = (base_socket_t *)sock;

    ocnet_event_destroy(base_sock->event);
    ocnet_socket_close(base_sock->sock);
    ocnet_free(base_sock);
}

int base_socket_event_enroll(void *sock, void *evgrp)
{
    base_socket_t *base_sock = (base_socket_t *)sock;
    return ocnet_evgrp_event_add(evgrp, base_sock->event);
}

int base_socket_event_wait(void *sock, void *lfds, int timeout)
{
    base_socket_t *base_sock = (base_socket_t *)sock;
    return ocnet_event_wait(base_sock->event, lfds, timeout);
}

int base_socket_event_del(void *sock, void *evgrp)
{
    base_socket_t *base_sock = (base_socket_t *)sock;
    return ocnet_evgrp_event_del(evgrp, base_sock->event);
}

int base_socket_readable(void *sock, void *lfds)
{
    base_socket_t *base_sock = (base_socket_t *)sock;
    return ocnet_event_happen(base_sock->event, lfds, OCNET_EVENT_READ);
}

int base_socket_sendto(void *sock, char *buf, int len,
        ocnet_ip_t ip, ocnet_port_t port)
{
    base_socket_t *base_sock = (base_socket_t *)sock;
    return ocnet_socket_sendto(base_sock->sock, buf, len, ip, port);
}

int base_socket_recvfrom(void *sock, char *buf, int len,
        ocnet_ip_t *ip, ocnet_port_t *port)
{
    base_socket_t *base_sock = (base_socket_t *)sock;
    return ocnet_socket_recvfrom(base_sock->sock, buf, len, ip, port);
}

int base_socket_send(void *sock, ocnet_conn_t conn, char *buf, int len)
{
    base_socket_t *base_sock = (base_socket_t *)sock;
    return ocnet_socket_send(base_sock->sock, conn, buf, len);
}

int base_socket_recv(void *sock, ocnet_conn_t conn, char *buf, int len)
{
    base_socket_t *base_sock = (base_socket_t *)sock;
    return ocnet_socket_recv(base_sock->sock, conn, buf, len);
}

int base_socket_listen(void *sock, int backlog)
{
    base_socket_t *base_sock = (base_socket_t *)sock;
    return ocnet_socket_listen(base_sock->sock, backlog);
}

ocnet_conn_t base_socket_accept(void *sock, ocnet_ip_t *ip, ocnet_port_t *port)
{
    base_socket_t *base_sock = (base_socket_t *)sock;
    return ocnet_socket_accept(base_sock->sock, ip, port);
}

ocnet_conn_t base_socket_connect(void *sock, ocnet_ip_t ip, ocnet_port_t port)
{
    base_socket_t *base_sock = (base_socket_t *)sock;
    return ocnet_socket_connect(base_sock->sock, ip, port);
}

int base_socket_close_connection(void *sock, ocnet_conn_t conn)
{
    base_socket_t *base_sock = (base_socket_t *)sock;
    return ocnet_socket_close_connection(base_sock->sock, conn);
}

ocnet_ipvx_t base_socket_get_ipvx(void *sock)
{
    base_socket_t *base_sock = (base_socket_t *)sock;
    return base_sock->ip_version;
}

ocnet_ip_t base_socket_get_ip(void *sock)
{
    base_socket_t *base_sock = (base_socket_t *)sock;
    return base_sock->ip;
}

ocnet_port_t base_socket_get_port(void *sock)
{
    base_socket_t *base_sock = (base_socket_t *)sock;
    return base_sock->port;
}
