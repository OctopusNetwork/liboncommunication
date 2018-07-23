#include <stdio.h>
#include <unistd.h>

#include "on_iport.h"

#include "udp_socket.h"

int main(int argc, char *argv[])
{
    onc_ip_t ip = onc_iport_parse_ip("127.0.0.1");
    onc_port_t port = 10687;
    void *udp_sock = NULL;
    int recv_result = 0;
    char buf[1024] = {0};

    udp_sock = udp_socket_new(0, port);
    if (NULL == udp_sock) {
        return -1;
    }

    udp_socket_sendto(udp_sock, "hello", 6,
            onc_iport_parse_ip("127.0.0.1"), 10688);

    do {
        recv_result = udp_socket_recvfrom(udp_sock,
                buf, 1024, &ip, &port);
        if (0 < recv_result) {
            printf("Rev from %d.%d.%d.%d:%d <%s>\n",
                    ip >> 24 & 0xff, ip >> 16 & 0xff,
                    ip >> 8 & 0xff, ip & 0xff,
                    port, buf);
            break;
        }

        usleep(20 * 1000 * 1000);
    } while (1);

    udp_socket_del(udp_sock);

    return 0;
}
