#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../Server/dhcp_message.h"

#define Client_port_DHCP 1068
#define Server_port_DHCP 1067
#define Buffer_size 1024


int main()
{
    int sockfd;
    struct sockaddr_in broadcast_addr, my_addr;
    
    // creez socket-ul udp
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("Eroare la crearea socketului");
        exit(EXIT_FAILURE);
    }

    // discover e broadcast
    int broadcastPerm = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastPerm, sizeof(broadcastPerm)) < 0)
    {
        perror("Eroare la setarea broadcastului");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // configurez  adresa clientului
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons(Client_port_DHCP);

   if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0)
    {
        perror("Eroare la bind client");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    //conf adresa broadcast
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(Server_port_DHCP);
    broadcast_addr.sin_addr.s_addr = inet_addr("255.255.255.255");

    // var initiala de test - folosita pentru verificarea broadcastului
    //snprintf(buffer, sizeof(buffer), "dhcp dicover primit de la %d", getpid());
    //sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr));

    // var "oficiala" - folosita in implementarea protocolului DHCP
    // DHCP DISCOVER
    DHCP_Message discover;
    memset(&discover, 0, sizeof(discover));
    discover.header.op = 1;
    discover.header.htype = 1;
    discover.header.hlen = 6;
    discover.header.xid = rand();
    discover.msg_type = DHCP_DISCOVER;

    sendto(sockfd, &discover, sizeof(DHCP_Message), 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr));

    printf("[CLIENT] DHCP DISCOVER trimis (xid=%u)\n", discover.header.xid);

    // DHCP OFFER
    DHCP_Message offer;
    int n = recvfrom(sockfd, &offer, sizeof(offer), 0, NULL, NULL);
    
    if (n < sizeof(DHCP_Message)) 
    {
        printf("[CLIENT] Mesaj prea scurt\n");
    } else if (offer.msg_type == DHCP_OFFER) 
    {
        printf("[CLIENT] DHCP OFFER primit: IP=%s, Router=%s, DNS=%s\n",
               offer.offered_ip, offer.router, offer.dns);
    }


    close(sockfd);
    return 0;
}
