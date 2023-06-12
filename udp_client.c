#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "172.26.0.1"
#define PORT 1234

int main() {
    int client_socket, bytes_sent;
    struct sockaddr_in server_address;

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        perror("Failed to create socket");
        exit(1);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);

    char reg_num[100];
    char serial_num[100];

    printf("Enter registration number: ");
    fgets(reg_num, sizeof(reg_num), stdin);
    reg_num[strcspn(reg_num, "\n")] = '\0';

    printf("Enter serial number: ");
    fgets(serial_num, sizeof(serial_num), stdin);
    serial_num[strcspn(serial_num, "\n")] = '\0';

    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%s,%s", reg_num, serial_num);

    bytes_sent = sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&server_address, sizeof(server_address));
    if (bytes_sent == -1) {
        perror("Failed to send data");
        exit(1);
    }

    char response[1024];
    memset(response, 0, sizeof(response));

    bytes_sent = recvfrom(client_socket, response, sizeof(response), 0, NULL, NULL);
    if (bytes_sent == -1) {
        perror("Failed to receive response");
        exit(1);
    }

    printf("Response from server: %s\n", response);

    close(client_socket);
    return 0;
}
