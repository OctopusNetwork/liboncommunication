#include <stdio.h>

#include "tcp_socket.h"
#include "tcp_socket_internal.h"
#include "ocnet_iport.h"
#include "ocnet_malloc.h"

#include "onlfds.h"

int main(int argc, char *argv[])
{
    void *tcp_sock = NULL;
    ocnet_ip_t ip;
    ocnet_port_t port;
    ocnet_conn_t ocnet_conn = 0;
    int recv_result = 0;
    char buf[1024] = {0};

    tcp_sock = tcp_socket_new(0, 10688, 1000,
            OCNET_EVENT_READ | OCNET_EVENT_ERROR);
    if (NULL == tcp_sock) {
        return -1;
    }

    if (tcp_socket_listen(tcp_sock) < 0) {
        tcp_socket_del(tcp_sock);
        return -1;
    }

    ocnet_conn = tcp_socket_accept(tcp_sock, &ip, &port);
    if (ocnet_conn < 0) {
        tcp_socket_del(tcp_sock);
        return -1;
    } else {
        printf("Connect from: %d.%d.%d.%d:%d\n",
                ip >> 24 & 0xff, ip >> 16 & 0xff,
                ip >> 8 & 0xff, ip & 0xff, port);
    }

    do {
        recv_result = tcp_socket_recv(tcp_sock,
                ocnet_conn, buf, sizeof(buf));
        if (0 < recv_result) {
            printf("Client msg: <%s>\n", buf);
            tcp_socket_send(tcp_sock, ocnet_conn, "world", 6);
            break;
        }
        usleep(100 * 1000);
    } while (1);

    tcp_socket_close_connection(tcp_sock, ocnet_conn);
    tcp_socket_del(tcp_sock);
    return 0;
}
