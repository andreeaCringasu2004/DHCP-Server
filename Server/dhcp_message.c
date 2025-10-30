#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include "dhcp_message.h"

// Algoritmul de alocare/"gemerare" al ip-urilor
int allocate_ip(IP_Entry *ip_pool, int pool_size, char *assigned_ip, int default_lease)
{
    time_t now = time(NULL);
    for (int i = 0; i < pool_size; i++)
    {
        if (!ip_pool[i].allocated || (ip_pool[i].lease_start + ip_pool[i].lease_time < now))
        {
            // pastram ip-ul;
            ip_pool[i].allocated = 1;
            ip_pool[i].lease_start = now;
            ip_pool[i].lease_time = default_lease;
            strcpy(assigned_ip, ip_pool[i].ip);
            return 1; // -> SUCCES
        }
    }
    return 0; // nu mai sunt IP-uri libere
}

// Gestionare DHCP DISCOVER
void handle_dhcp_discover(int sockfd, struct sockaddr_in *client_addr,
                          DHCP_Message *discover, DHCP_ipconfig *conf, IP_Entry *ip_pool, int pool_size)
{
    printf("[SERVER] Procesare DHCP DISCOVER (xid=%u)...\n", discover->header.xid);
    send_dhcp_offer(sockfd, client_addr, discover, conf, ip_pool, pool_size);
}

// Trimiterea unui DHCP OFFER
void send_dhcp_offer(int sockfd, struct sockaddr_in *client_addr,
                     DHCP_Message *discover, DHCP_ipconfig *conf, IP_Entry *ip_pool, int pool_size)
{
    DHCP_Message offer;
    memset(&offer, 0, sizeof(offer));

    offer.header.op = 2;
    offer.header.htype = 1;
    offer.header.hlen = 6;
    offer.header.xid = discover->header.xid;
    offer.header.siaddr = inet_addr(conf->range_start);

    offer.msg_type = DHCP_OFFER;
    offer.lease_time = conf->default_lease;
    strcpy(offer.router, conf->router);
    strcpy(offer.dns, conf->dns);

    if (!allocate_ip(ip_pool, pool_size, offer.offered_ip, conf->default_lease)) {
        printf("[SERVER] Nu mai sunt IP-uri disponibile!\n");
        return;
    }

    offer.header.yiaddr = inet_addr(offer.offered_ip);

    sendto(sockfd, &offer, sizeof(offer), 0,
           (struct sockaddr *)client_addr, sizeof(*client_addr));

    printf("[SERVER] DHCP OFFER trimis către %s → IP %s\n",
           inet_ntoa(client_addr->sin_addr), offer.offered_ip);
}