#include "on_malloc.h"

#include "on_log.h"

#include "onlfds.h"
#include "onevent.h"
#include "onevgrp.h"

#include "tcp_connection.h"

#include "tcp_socket_internal.h"

#define TAG  "tcp,connection"

typedef struct {
    void        *tcp_sock;
    onc_conn_t   conn;
    void        *event;
    onc_ip_t     ip;
    onc_port_t   port;
} tcp_connection_s_t;

static int __event_wait(void *sock, int event_wait,
        void *lfds, int millseconds)
{
    int rc = 0;

    if (0 != event_wait) {
        void *sock_lfds = NULL;
        if (NULL == lfds) {
            sock_lfds = onc_lfds_new();
            if (NULL != sock_lfds) {
                lfds = sock_lfds;
            } else {
                return -1;
            }
        }
        rc = tcp_socket_event_wait(sock, lfds, millseconds);
        if (NULL != sock_lfds) {
            onc_lfds_del(sock_lfds);
        }
    } else {
        if (NULL == lfds) {
            return -1;
        }
        rc = tcp_socket_acceptable(sock, lfds);
    }

    return rc;
}

static void *__connection_create(void *sock, onc_conn_t conn,
        onc_ip_t ip, onc_port_t port, int events)
{
    tcp_connection_s_t *tcp_conn = NULL;

    tcp_conn = onc_malloc(sizeof(tcp_connection_s_t));
    if (NULL == tcp_conn) {
        tcp_socket_close_connection(sock, conn);
        return NULL;
    }

    tcp_conn->event = onc_event_create(0, events, 0, conn, conn);
    if (NULL == tcp_conn->event) {
        tcp_socket_close_connection(sock, conn);
        onc_free(tcp_conn);
        return NULL;
    }

    tcp_conn->ip = ip;
    tcp_conn->port = port;
    tcp_conn->conn = conn;
    tcp_conn->tcp_sock = sock;

    return tcp_conn;
}

static void __connection_destroy(tcp_connection_s_t *conn)
{
    onc_event_destroy(conn->event);
    tcp_socket_close_connection(conn->tcp_sock, conn->conn);
    onc_free(conn);
}

void *__accept(void *sock)
{
    onc_ip_t ip;
    onc_port_t port;
    onc_conn_t conn;
    char ip_desc[32] = {0};
    char port_desc[8] = {0};

    conn = tcp_socket_accept(sock, &ip, &port);
    if (KKT_CONN_INVALID == conn) {
        return NULL;
    }

    onc_iport_ip_todesc(ip, ip_desc, sizeof(ip_desc));
    onc_iport_port_todesc(port, port_desc, sizeof(port_desc));
    LOGI(TAG, "connection from %s:%s", ip_desc, port_desc);

    return __connection_create(sock, conn, ip, port,
            KKT_EVENT_READ | KKT_EVENT_ERROR);
}

void *tcp_connection_accept(void *sock, int event_wait,
        void *lfds, int millseconds)
{
    int rc = 0;

    rc = __event_wait(sock, event_wait, lfds, millseconds);
    if (rc < 0) {
        return NULL;
    } else if (0 < rc) {
        return __accept(sock);
    } else {
        return NULL;
    }

    return NULL;
}

onc_ip_t tcp_connection_get_ip(void *tcp_conn)
{
    tcp_connection_s_t *conn = (tcp_connection_s_t *)tcp_conn;
    return conn->ip;
}

onc_ip_t tcp_connection_get_port(void *tcp_conn)
{
    tcp_connection_s_t *conn = (tcp_connection_s_t *)tcp_conn;
    return conn->port;
}

void tcp_connection_wait_writable(void *tcp_conn)
{
    tcp_connection_s_t *conn = (tcp_connection_s_t *)tcp_conn;
    onc_event_add(conn->event, KKT_EVENT_WRITE);
}

void tcp_connection_dontwait_writable(void *tcp_conn)
{
    tcp_connection_s_t *conn = (tcp_connection_s_t *)tcp_conn;
    onc_event_del(conn->event, KKT_EVENT_WRITE);
}

void *tcp_connection_connect(void *sock, onc_ip_t ip, onc_port_t port)
{
    onc_conn_t conn;

    conn = tcp_socket_connect(sock, ip, port);
    if (KKT_CONN_INVALID == conn) {
        return NULL;
    }

    return __connection_create(sock, conn, ip, port,
            KKT_EVENT_READ | KKT_EVENT_ERROR);
}

void tcp_connection_del(void *tcp_conn)
{
    tcp_connection_s_t *conn = (tcp_connection_s_t *)tcp_conn;
    char ip_desc[32] = {0};
    char port_desc[8] = {0};

    onc_iport_ip_todesc(conn->ip, ip_desc, sizeof(ip_desc));
    onc_iport_port_todesc(conn->port, port_desc, sizeof(port_desc));
    LOGI(TAG, "close connection %s:%s", ip_desc, port_desc);

    __connection_destroy(conn);
}

int tcp_connection_event_enroll(void *tcp_conn, void *evgrp)
{
    tcp_connection_s_t *conn = (tcp_connection_s_t *)tcp_conn;
    return onc_evgrp_event_add(evgrp, conn->event);
}

int tcp_connection_event_del(void *tcp_conn, void *evgrp)
{
    tcp_connection_s_t *conn = (tcp_connection_s_t *)tcp_conn;
    return onc_evgrp_event_del(evgrp, conn->event);
}

int tcp_connection_event_wait(void *tcp_conn, void *lfds, int timeout)
{
    tcp_connection_s_t *conn = (tcp_connection_s_t *)tcp_conn;
    return onc_event_wait(conn->event, lfds, timeout);
}

int tcp_connection_readable(void *tcp_conn, void *lfds)
{
    tcp_connection_s_t *conn = (tcp_connection_s_t *)tcp_conn;
    return onc_event_happen(conn->event, lfds, KKT_EVENT_READ);
}

int tcp_connection_read(void *tcp_conn, char *buf, int size)
{
    tcp_connection_s_t *conn = (tcp_connection_s_t *)tcp_conn;
    return tcp_socket_recv(conn->tcp_sock, conn->conn, buf, size);
}

int tcp_connection_writable(void *tcp_conn, void *lfds)
{
    tcp_connection_s_t *conn = (tcp_connection_s_t *)tcp_conn;
    return onc_event_happen(conn->event, lfds, KKT_EVENT_WRITE);
}

int tcp_connection_write(void *tcp_conn, char *buf, int size)
{
    tcp_connection_s_t *conn = (tcp_connection_s_t *)tcp_conn;
    return tcp_socket_send(conn->tcp_sock, conn->conn, buf, size);
}

int tcp_connection_error(void *tcp_conn, void *lfds)
{
    tcp_connection_s_t *conn = (tcp_connection_s_t *)tcp_conn;
    return onc_event_happen(conn->event, lfds, KKT_EVENT_ERROR);
}

