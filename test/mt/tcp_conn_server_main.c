#include <stdio.h>

#include "logger.h"

#include "tcp_socket_internal.h"
#include "ocnet_iport.h"
#include "ocnet_malloc.h"

#include "onlfds.h"

#include "tcp_connection.h"

int main(int argc, char *argv[])
{
    void *tcp_sock = NULL;
    void *tcp_conn = NULL;
    ocnet_ip_t ip;
    ocnet_port_t port;
    int recv_result = 0;
    char buf[1024] = {0};
    void *lfds = NULL;
    int rc = 0;

    tcp_sock = tcp_socket_new(0, 10688, 1000,
            OCNET_EVENT_READ | OCNET_EVENT_ERROR);
    if (NULL == tcp_sock) {
        return -1;
    }

    if (tcp_socket_listen(tcp_sock) < 0) {
        tcp_socket_del(tcp_sock);
        return -1;
    }

    lfds = ocnet_lfds_new();

    do {
        rc = tcp_socket_event_wait(tcp_sock, lfds, 1000);
        if (rc < 0) {
        } else if (0 < rc) {
            tcp_conn = tcp_connection_accept(tcp_sock, 0, lfds, 1000);
            if (NULL != tcp_conn) {
                ip = tcp_connection_get_ip(tcp_conn);
                port = tcp_connection_get_port(tcp_conn);
                printf("Connect from: %d.%d.%d.%d:%d\n",
                        ip >> 24 & 0xff, ip >> 16 & 0xff,
                        ip >> 8 & 0xff, ip & 0xff, port);
                break;
            } else {
                tcp_socket_del(tcp_sock);
                return -1;
            }
        } else {
            printf("Timeout---\n");
        }
    } while (1);

    do {
        rc = tcp_connection_event_wait(tcp_conn, lfds, 1000);
        if (rc < 0) {

        } else if (0 < rc) {
            if (0 != tcp_connection_readable(tcp_conn, lfds)) {
                recv_result = tcp_connection_read(tcp_conn, buf, sizeof(buf));
                if (0 < recv_result) {
                    printf("Client msg: <%s>\n", buf);
                    tcp_connection_write(tcp_conn, "world", 6);
                    break;
                }
            }
        } else {

        }
        usleep(100 * 1000);
    } while (1);

    ocnet_lfds_del(lfds);
    tcp_connection_del(tcp_conn);
    tcp_socket_del(tcp_sock);
    return 0;
}
