#include <stdio.h>

#include "tcp_socket.h"
#include "tcp_socket_internal.h"
#include "ocnet_iport.h"
#include "ocnet_malloc.h"

#include "onlfds.h"

int main(int argc, char *argv[])
{
    void *tcp_sock = NULL;
    ocnet_conn_t ocnet_conn;
    int recv_result = 0;
    char buf[1024] = {0};

    tcp_sock = tcp_socket_new(OCNET_SOCK_TCP, 0, 10687,
            OCNET_EVENT_READ | OCNET_EVENT_ERROR);
    if (NULL == tcp_sock) {
        return -1;
    }

    ocnet_conn = tcp_socket_connect(tcp_sock,
            ocnet_iport_parse_ip("127.0.0.1"), 10688);
    if (ocnet_conn < 0) {
        tcp_socket_del(tcp_sock);
        return -1;
    } else {
        printf("Connect to: 127.0.0.1:10688\n");
    }

    if (tcp_socket_send(tcp_sock, ocnet_conn, "hello", 6) < 0) {
        tcp_socket_close_connection(tcp_sock, ocnet_conn);
        tcp_socket_del(tcp_sock);
        printf("Fail to write msg\n");
        return -1;
    }

    do {
        recv_result = tcp_socket_recv(tcp_sock,
                ocnet_conn, buf, sizeof(buf));
        if (0 < recv_result) {
            printf("Server msg: <%s>\n", buf);
            break;
        }
        usleep(100 * 1000);
    } while (1);

    tcp_socket_del(tcp_sock);
    return 0;
}
