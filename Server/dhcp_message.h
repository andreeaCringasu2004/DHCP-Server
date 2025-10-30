#ifndef DHCP_MESSAGE_H
#define DHCP_MESSAGE_H

#include <stdint.h>
#include <arpa/inet.h>
#include "config.h"

// Codurile pentru tipuile de mesaje DHCP
#define DHCP_DISCOVER 1
#define DHCP_OFFER    2
#define DHCP_REQUEST  3
#define DHCP_DECLINE  4
#define DHCP_ACK      5
#define DHCP_NAK      6
#define DHCP_RELEASE  7


//Strctura comuna pentru toate mesajele DHCP

//header-ul dhcp - partea fixa a mesajului
typedef struct {
    uint8_t  op;         // 1 = request, 2 = reply
    uint8_t  htype;      // tip hardware (1 = Ethernet)
    uint8_t  hlen;       // lungimea adresei hardware (6 pentru MAC)
    uint8_t  hops;       // număr de relay (0 în general)
    uint32_t xid;        // transaction ID (identificator unic sesiune)
    uint16_t secs;       // secunde de la începutul cererii
    uint16_t flags;      // 0x8000 = broadcast flag
    uint32_t ciaddr;     // IP curent al clientului (dacă are deja)
    uint32_t yiaddr;     // IP oferit clientului („your IP address”)
    uint32_t siaddr;     // IP server DHCP
    uint32_t giaddr;     // IP relay agent (0 dacă nu există)
    unsigned char chaddr[16];  // adresa MAC client (6 octeți reali + rest zero)
    char sname[64];      // nume server (opțional)
    char file[128];      // fișier boot (opțional)
}DHCP_Header;

typedef struct {
    DHCP_Header header;  // partea fixă, comună tuturor mesajelor
    int msg_type;        // tipul mesajului DHCP (DISCOVER, OFFER etc.)
    char offered_ip[16]; // IP oferit clientului (string)
    char router[16];     // IP router/gateway
    char dns[16];        // IP server DNS
    int lease_time;      // durata lease-ului
}DHCP_Message;


// structura pt evidenta IP-urilor
typedef struct {
    char ip[16];
    int allocated;      // 1 = ocupat, 0 = liber
    time_t lease_start;
    int lease_time;
} IP_Entry;


void handle_dhcp_discover(int sockfd, struct sockaddr_in *client_addr,
                          DHCP_Message *discover, DHCP_ipconfig *conf, IP_Entry *ip_pool, int pool_size);

void send_dhcp_offer(int sockfd, struct sockaddr_in *client_addr,
                     DHCP_Message *discover, DHCP_ipconfig *conf, IP_Entry *ip_pool, int pool_size);

int allocate_ip(IP_Entry *ip_pool, int pool_size, char *assigned_ip, int default_lease);


#endif