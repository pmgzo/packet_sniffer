#include<linux/if_packet.h> //AF_PACKET
#include<linux/if_ether.h> //structs
#include <sys/socket.h>
#include <stdlib.h>
#include<net/if.h> //itf
#include <strings.h> //bzero
#include <string.h>
#include <stdio.h>

#include<sys/ioctl.h>
#include <errno.h>

#include <arpa/inet.h> //htons


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// #include <netinet/in.h>


int bind_sock_on_itf(char *device, int rawsock, int protocol)
{
	struct sockaddr_ll sll;
	struct ifreq ifr;

	bzero(&sll, sizeof(sll));
	bzero(&ifr, sizeof(ifr));
	
	/* First Get the Interface Index  */

	strncpy((char *)ifr.ifr_name, device, IFNAMSIZ);
	if((ioctl(rawsock, SIOCGIFINDEX, &ifr)) == -1)//fill iterface's index
	{
		printf("Error getting Interface index !\n");
		exit(-1);
	}

	/* Bind our raw socket to this interface */

	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifr.ifr_ifindex;
	sll.sll_protocol = htons(protocol); 


	if((bind(rawsock, (struct sockaddr *)&sll, sizeof(sll)))== -1)
	{
		perror("Error binding raw socket to interface\n");
		exit(-1);
	}

	if(ioctl(rawsock,SIOCGIFADDR,&ifr) < 0) //getting IP Address
	{
		printf("error in SIOCGIFADDR \n");
	}
	struct sockaddr_in a  = *(struct sockaddr_in*)&ifr.ifr_addr;
	// ifr.ifr_addr;
	printf("your ip addr is %s\n", inet_ntoa(a.sin_addr));


	return 1;
	
}
