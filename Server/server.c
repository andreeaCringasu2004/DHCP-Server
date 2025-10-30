#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "config.h"
#include "dhcp_message.h"


#define Server_port_DHCP 1067
#define Buffer_size 1024
#define Pool_size 100

int main() 
{   
    //incarca in sistem configuratia serverului
    DHCP_ipconfig configuratie;
    if (load_config("Server/ipconfig.txt", &configuratie) == -1) {
        fprintf(stderr, "nu s-a incarcat configuratia serverului\n");
        return 1;
    }

    //afisare configurare server
    
    printf("subnet: %s  %s\n", configuratie.subnet, configuratie.netmask);
    printf("range: %s  %s\n", configuratie.range_start, configuratie.range_end);
    printf("router: %s\n", configuratie.router);
    printf("DNS: %s\n", configuratie.dns);
    printf("lease def: %d s\n", configuratie.default_lease);
    printf("lease max: %d s\n", configuratie.max_lease);


    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    //char buffer[Buffer_size];
    socklen_t addr_len = sizeof(client_addr);

    // creez socket-ul udp
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("Eroare la crearea socketului");
        exit(EXIT_FAILURE);
    }


    //optiunea 0 imi permite reutilizarea portului
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    //configurez adresa serverului
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(Server_port_DHCP);

    //conectez socket-ul de port
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("Eroare la bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Serverul DHCP pornit pe %d\n", Server_port_DHCP);

    IP_Entry ip_pool[Pool_size];
    struct in_addr ip;
    inet_aton(configuratie.range_start, &ip);
    for (int i = 0; i < Pool_size; i++)
    {
        ip_pool[i].allocated = 0;
        ip_pool[i].lease_start = 0;
        ip_pool[i].lease_time = 0;

        struct in_addr temp_ip;
        temp_ip.s_addr = htonl(ntohl(ip.s_addr) + i);
        strcpy(ip_pool[i].ip, inet_ntoa(temp_ip));
    }

    // primesc mesajele de la client
    while (1) 
    {
        DHCP_Message msg_recv;
        int n = recvfrom(sockfd, &msg_recv, sizeof(msg_recv), 0, 
            (struct sockaddr *)&client_addr, &addr_len);
        
        if (n < sizeof(DHCP_Message))
            continue;

        //DHCP discover
        
        if (msg_recv.msg_type == DHCP_DISCOVER)
        {
            printf("[SERVER] DHCP DISCOVER primit (xid=%u)\n", msg_recv.header.xid);
            handle_dhcp_discover(sockfd, &client_addr, &msg_recv, &configuratie, ip_pool, Pool_size);
        }
        else
        {
            printf("[SERVER] Mesaj necunoscut: %d\n", msg_recv.msg_type);
        }
    }

    close(sockfd);
    return 0;
}
