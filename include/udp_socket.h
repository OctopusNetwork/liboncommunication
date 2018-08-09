#ifndef __ONC_UDP_SOCKET____H__
#define __ONC_UDP_SOCKET____H__

#include "ocnet_iport.h"

#ifdef __cplusplus
extern "C" {
#endif

void *udp_socket_new(ocnet_ip_t ip, ocnet_port_t port);

int   udp_socket_sendto(void *socket, char *buf, int len,
        ocnet_ip_t ip, ocnet_port_t port);
int   udp_socket_recvfrom(void *socket, char *buf, int len,
        ocnet_ip_t *ip, ocnet_port_t *port);

int   udp_socket_event_enroll(void *sock, void *evgrp);
int   udp_socket_event_del(void *sock, void *evgrp);
int   udp_socket_readable(void *sock, void *lfds);

ocnet_ipvx_t      udp_socket_get_ipvx(void *socket);
ocnet_ip_t        udp_socket_get_ip(void *socket);
ocnet_port_t      udp_socket_get_port(void *socket);

void  udp_socket_del(void *socket);

#ifdef __cplusplus
}
#endif

#endif
