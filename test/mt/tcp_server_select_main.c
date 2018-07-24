#include <stdio.h>

#include "tcp_socket.h"
#include "tcp_socket_internal.h"
#include "on_iport.h"
#include "on_malloc.h"

#include "onlfds.h"

int main(int argc, char *argv[])
{
    void *tcp_sock = NULL;
    onc_ip_t ip;
    onc_port_t port;
    onc_conn_t onc_conn = 0;
    int recv_result = 0;
    char buf[1024] = {0};
    void *lfds = NULL;

    tcp_sock = tcp_socket_new(0, 10688, 1000,
            KKT_EVENT_READ | KKT_EVENT_ERROR);
    if (NULL == tcp_sock) {
        return -1;
    }

    if (tcp_socket_listen(tcp_sock) < 0) {
        tcp_socket_del(tcp_sock);
        return -1;
    }

    lfds = onc_lfds_new();

    do {
        int rc = 0;
        rc = tcp_socket_event_wait(tcp_sock, lfds, 1000);
        if (rc < 0) {
        } else if (0 < rc) {
            onc_conn = tcp_socket_accept(tcp_sock, &ip, &port);
            if (KKT_CONN_INVALID != onc_conn) {
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
        recv_result = tcp_socket_recv(tcp_sock,
                onc_conn, buf, sizeof(buf));
        if (0 < recv_result) {
            printf("Client msg: <%s>\n", buf);
            tcp_socket_send(tcp_sock, onc_conn, "world", 6);
            break;
        }
        usleep(100 * 1000);
    } while (1);

    onc_lfds_del(lfds);

    tcp_socket_close_connection(tcp_sock, onc_conn);
    tcp_socket_del(tcp_sock);
    return 0;
}
