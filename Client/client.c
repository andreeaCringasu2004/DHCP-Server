#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define Server_ip "127.0.0.1"
#define Server_port 5000
#define Buffer_size 1024

int main()
{
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[Buffer_size];

    // creez socket-ul udp
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("Eroare la crearea socketului");
        exit(EXIT_FAILURE);
    }

    // configurez  adresa server-ului
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(Server_port);
    server_addr.sin_addr.s_addr = inet_addr(Server_ip);

    printf("Introdu mesajul pentru server: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    // trimit mesajul catre server
    sendto(sockfd, buffer, strlen(buffer), 0,
           (struct sockaddr *)&server_addr, sizeof(server_addr));
    
    // astept rezultatul
    int n = recvfrom(sockfd, buffer, Buffer_size, 0, NULL, NULL);
    buffer[n] = '\0';

    printf("Raspuns de la server: %s\n", buffer);

    close(sockfd);
    return 0;
}