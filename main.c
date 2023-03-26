#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

#include <sys/types.h>
#include <strings.h>
#include <stdlib.h>

#include <netinet/ip.h>
#include<linux/if_ether.h>
#include<linux/if_packet.h>
// #include <netinet/ip.h>

#include <netinet/tcp.h>
#include <netinet/udp.h>

// https://www.opensourceforu.com/2015/03/a-guide-to-using-raw-sockets/

int bind_sock_on_itf(char *device, int rawsock, int protocol);

int parse_tcp_hdr(struct tcphdr *packet);
int parse_udp_hdr(struct udphdr *packet);
int parse_eth_hdr(struct ethhdr *paquet);
int parse_ip_hdr(struct iphdr *paquet, int *protocol);

int read_message(unsigned char *buff, size_t len, int inc)
{
    for (size_t i = inc; i < len && buff[i]; i++) {
        if (i == 0 || i % 16 == 0)
            printf("\n");
        printf("%.2X ", buff[i]);
    }
    for (size_t i = inc; i < len && buff[i]; i++) {
        if (i == 0 || i % 16 == 0)
            printf("\n");
        printf("%c", buff[i]);
    }
    
    //add ascii printing
    return (0);
}

int get_paquet(int sock_r)
{
    size_t len = 65536;
    void *buff = malloc(len);
    bzero(buff, len);
    
    struct sockaddr saddr;
    socklen_t s_len = sizeof(struct sockaddr);
    int inc = 0;
    int protocol = 0;

    recvfrom(sock_r, buff, len, 0, &saddr, &s_len); //used to get much more information
    
    parse_eth_hdr(buff);
    inc += sizeof(struct ethhdr);
    inc += parse_ip_hdr(buff + inc, &protocol);

    if (protocol == 17) {
        parse_tcp_hdr(buff + inc);
        inc += sizeof(struct tcphdr);
    } else if (protocol == 6) {
        parse_udp_hdr(buff + inc);
        inc += sizeof(struct udphdr);
    }
    else {
        return (1);
    }
    read_message(buff, len, inc);
    return (0);
}

int main()
{
    int sock_r;

    sock_r = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); //ETH_P_ALL for catching all paquet - ETH_P_IP for ip paquet

    if (bind_sock_on_itf("wlp3s0", sock_r, ETH_P_ALL) < 0) {
        printf("binding failed\n");
        return (1);
    }
    if (!sock_r) {
        printf("fail to open raw sock\n");
        return (1);
    }
    get_paquet(sock_r);
    return (0);
}