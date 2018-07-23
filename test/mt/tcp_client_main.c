#include <stdio.h>

#include "tcp_socket.h"
#include "tcp_socket_internal.h"
#include "on_iport.h"
#include "on_malloc.h"

#include "onlfds.h"

int main(int argc, char *argv[])
{
    void *tcp_sock = NULL;
    onc_conn_t onc_conn;
    int recv_result = 0;
    char buf[1024] = {0};

    tcp_sock = tcp_socket_new(KKT_SOCK_TCP, 0, 10687,
            KKT_EVENT_READ | KKT_EVENT_ERROR);
    if (KKT_SOCKET_INVALID == tcp_sock) {
        return -1;
    }

    onc_conn = tcp_socket_connect(tcp_sock,
            onc_iport_parse_ip("127.0.0.1"), 10688);
    if (onc_conn == KKT_CONN_INVALID) {
        tcp_socket_del(tcp_sock);
        return -1;
    } else {
        printf("Connect to: 127.0.0.1:10688\n");
    }

    if (tcp_socket_send(tcp_sock, onc_conn, "hello", 6) < 0) {
        tcp_socket_close_connection(tcp_sock, onc_conn);
        tcp_socket_del(tcp_sock);
        printf("Fail to write msg\n");
        return -1;
    }

    do {
        recv_result = tcp_socket_recv(tcp_sock,
                onc_conn, buf, sizeof(buf));
        if (0 < recv_result) {
            printf("Server msg: <%s>\n", buf);
            break;
        }
        usleep(100 * 1000);
    } while (1);

    tcp_socket_del(tcp_sock);
    return 0;
}
