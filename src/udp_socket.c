#include "ocnet_malloc.h"
#include "ocnet_socket.h"
#include "ocnet_iport.h"

#include "onlfds.h"

#include "base_socket.h"

#include "udp_socket.h"

typedef struct {
    void    *base_sock;
} udp_socket_s_t;

void *udp_socket_new(ocnet_ip_t ip, ocnet_port_t port)
{
    void            *base_sock = NULL;
    udp_socket_s_t  *udp_sock = NULL;

    base_sock = base_socket_new(OCNET_SOCK_UDP, ip, port,
            OCNET_EVENT_READ | OCNET_EVENT_ERROR);
    if (NULL == base_sock) {
        return NULL;
    }

    udp_sock = (udp_socket_s_t *)ocnet_malloc(sizeof(udp_socket_s_t));
    if (NULL == udp_sock) {
        base_socket_del(base_sock);
        return NULL;
    }

    udp_sock->base_sock = base_sock;
    return udp_sock;
}

void udp_socket_del(void *sock)
{
    udp_socket_s_t *udp_sock = (udp_socket_s_t *)sock;
    base_socket_del(udp_sock->base_sock);
    ocnet_free(udp_sock);
}

int udp_socket_event_enroll(void *sock, void *evgrp)
{
    udp_socket_s_t *udp_sock = (udp_socket_s_t *)sock;
    return base_socket_event_enroll(udp_sock->base_sock, evgrp);
}

int udp_socket_event_del(void *sock, void *evgrp)
{
    udp_socket_s_t *udp_sock = (udp_socket_s_t *)sock;
    return base_socket_event_del(udp_sock->base_sock, evgrp);
}

int udp_socket_readable(void *sock, void *lfds)
{
    udp_socket_s_t *udp_sock = (udp_socket_s_t *)sock;
    return base_socket_readable(udp_sock->base_sock, lfds);
}

int udp_socket_sendto(void *sock, char *buf, int len,
        ocnet_ip_t ip, ocnet_port_t port)
{
    udp_socket_s_t *udp_sock = (udp_socket_s_t *)sock;
    return base_socket_sendto(udp_sock->base_sock, buf, len, ip, port);
}

int udp_socket_recvfrom(void *sock, char *buf, int len,
        ocnet_ip_t *ip, ocnet_port_t *port)
{
    udp_socket_s_t *udp_sock = (udp_socket_s_t *)sock;
    return base_socket_recvfrom(udp_sock->base_sock, buf, len, ip, port);
}

ocnet_ipvx_t udp_socket_get_ipvx(void *sock)
{
    udp_socket_s_t *udp_sock = (udp_socket_s_t *)sock;
    return base_socket_get_ipvx(udp_sock->base_sock);
}

ocnet_ip_t udp_socket_get_ip(void *sock)
{
    udp_socket_s_t *udp_sock = (udp_socket_s_t *)sock;
    return base_socket_get_ip(udp_sock->base_sock);
}

ocnet_port_t udp_socket_get_port(void *sock)
{
    udp_socket_s_t *udp_sock = (udp_socket_s_t *)sock;
    return base_socket_get_port(udp_sock->base_sock);
}
