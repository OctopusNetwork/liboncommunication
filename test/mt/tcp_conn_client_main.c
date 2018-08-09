#include <stdio.h>
#include <stdlib.h>

#include "tcp_socket.h"
#include "tcp_socket_internal.h"
#include "ocnet_iport.h"
#include "ocnet_malloc.h"

#include "onlfds.h"

#include "tcp_connection.h"

int main(int argc, char *argv[])
{
    void *tcp_sock = NULL;
    void *tcp_conn = NULL;
    int recv_result = 0;
    char buf[1024] = {0};
    int port = 10688;
    void *lfds = NULL;

    if (2 <= argc) {
        port = atoi(argv[1]);
    }

    tcp_sock = tcp_socket_new(OCNET_SOCK_TCP, 0, 10687,
            OCNET_EVENT_READ | OCNET_EVENT_ERROR);
    if (NULL == tcp_sock) {
        return -1;
    }

    tcp_conn = tcp_connection_connect(tcp_sock,
            ocnet_iport_parse_ip("127.0.0.1"), port);
    if (NULL == tcp_conn) {
        tcp_socket_del(tcp_sock);
        return -1;
    } else {
        printf("Connect to: 127.0.0.1:%d\n", port);
    }

    if (tcp_connection_write(tcp_conn, "hello", 6) < 0) {
        tcp_connection_del(tcp_conn);
        tcp_socket_del(tcp_sock);
        printf("Fail to write msg\n");
        return -1;
    }

    lfds = ocnet_lfds_new();

    do {
        int rc = 0;
        rc = tcp_connection_event_wait(tcp_conn, lfds, 1000);
        if (rc < 0) {

        } else if (0 < rc) {
            if (0 != tcp_connection_readable(tcp_conn, lfds)) {
                recv_result = tcp_connection_read(tcp_conn, buf, sizeof(buf));
                if (0 < recv_result) {
                    printf("Server msg: <%s>\n", buf);
                    break;
                }
            }
        } else {
            printf("Timeout---\n");
        }
        usleep(100 * 1000);
    } while (1);

    ocnet_lfds_del(lfds);

    tcp_socket_del(tcp_sock);
    return 0;
}
