#include <netinet/tcp.h>
#include <netinet/udp.h>

#include <netinet/ip.h>
#include<linux/if_ether.h>

#include <strings.h>
#include <stdio.h>

#include <arpa/inet.h> //nthos
#include <sys/socket.h>
#include <netinet/in.h> //inet_ntoa

int parse_tcp_hdr(struct tcphdr *packet)
{
    printf("src port = %d\n", ntohs(packet->source));
    printf("dst port = %d\n", ntohs(packet->dest));

    //http://www.firewall.cx/networking-topics/protocols/tcp/137-tcp-window-size-checksum.html#:~:text=The%20Window%20size%20is%20considered,always%20exist%20and%20be%20used.
    //packet->window used by the receiver to specify the amount of data that he can receive
    //packet->checksum ensures that the packet is arrived at destination (checksum is made is a trick with the pseudo headert(located in iphdr) more info here)
    //packet->urg_ptr specify the end address of the urgent data contained the message
    return (0);
}

int parse_udp_hdr(struct udphdr *packet)
{
    printf("src port = %d\n", ntohs(packet->source));
    printf("dst port = %d\n", ntohs(packet->dest));

    return (0);
}

int parse_eth_hdr(struct ethhdr *paquet)
{
    printf("Source MacAddress : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",paquet->h_source[0],paquet->h_source[1],paquet->h_source[2],paquet->h_source[3],paquet->h_source[4],paquet->h_source[5]);
    printf("Dest MacAddress : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",paquet->h_dest[0],paquet->h_dest[1],paquet->h_dest[2],paquet->h_dest[3],paquet->h_dest[4],paquet->h_dest[5]);
    return 0;
}

int parse_ip_hdr(struct iphdr *paquet, int *protocol)
{
    int ihl = paquet->ihl * 4; //header size
    struct sockaddr_in src;
    struct sockaddr_in dst;

    bzero(&src, sizeof(struct sockaddr_in));
    bzero(&dst, sizeof(struct sockaddr_in));

    src.sin_addr.s_addr = paquet->saddr;
    dst.sin_addr.s_addr = paquet->daddr;
    printf("src ip addr %s\n", inet_ntoa(src.sin_addr));
    printf("dst ip addr %s\n", inet_ntoa(dst.sin_addr));

    *protocol = paquet->protocol;
    if (paquet->protocol == 17)
        printf("TCP protocol\n");
    else if (paquet->protocol == 6)
        printf("UDP protocol\n");
    else
        printf("unknown protocol = %d\n", paquet->protocol);
    return (ihl);
}
