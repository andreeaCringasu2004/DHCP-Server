#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define Server_port 5000
#define Buffer_size 1024

int main() 
{
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[Buffer_size];
    socklen_t addr_len = sizeof(client_addr);

    // creez socket-ul udp
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("Eroare la crearea socketului");
        exit(EXIT_FAILURE);
    }

    //configurez adresa serverului
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(Server_port);

    //conectez socket-ul de port
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("Eroare la bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Serverul UDP ruleaza pe portul %d...\n", Server_port);

    // primesc mesajele de la client
    while (1) 
    {
        int n = recvfrom(sockfd, buffer, Buffer_size, 0, 
            (struct sockaddr *)&client_addr, &addr_len);
        buffer[n] = '\0';

        printf("Primit de la client: %s\n", buffer);

        // Trimit un raspuns
        char response[Buffer_size];
        snprintf(response, sizeof(response), "Salut client, am primit: %s", buffer);

        sendto(sockfd, response, strlen(response), 0,
               (struct sockaddr *)&client_addr, addr_len);
    }

    close(sockfd);
    return 0;
}