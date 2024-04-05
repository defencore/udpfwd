#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 65507
#define UDP_PORT 4000

// Define the destinations where the received stream will be forwarded
struct Destination {
    char *ip;
    int port;
};

struct Destination destinations[] = {
    {"127.0.0.1", 3000},
    {"127.0.0.1", 3001}
};

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[MAX_BUFFER_SIZE];

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(UDP_PORT);

    // Bind socket to the server address
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP server started and listening on port %d\n", UDP_PORT);

    // Continuously listen for incoming packets
    while (1) {
        ssize_t recv_len = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0,
                                     (struct sockaddr *)&client_addr, &client_len);
        if (recv_len < 0) {
            perror("Error receiving data");
            continue;
        }

        printf("Received %zd bytes from %s:%d\n", recv_len,
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Forward the received packet to all destinations
        for (size_t i = 0; i < sizeof(destinations) / sizeof(destinations[0]); ++i) {
            struct sockaddr_in dest_addr;
            memset(&dest_addr, 0, sizeof(dest_addr));
            dest_addr.sin_family = AF_INET;
            dest_addr.sin_addr.s_addr = inet_addr(destinations[i].ip);
            dest_addr.sin_port = htons(destinations[i].port);

            ssize_t sent_len = sendto(sockfd, buffer, recv_len, 0,
                                      (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            if (sent_len < 0) {
                perror("Error forwarding data");
                continue;
            }

            printf("Forwarded to %s:%d\n", destinations[i].ip, destinations[i].port);
        }
    }

    // Close socket
    close(sockfd);

    return 0;
}
