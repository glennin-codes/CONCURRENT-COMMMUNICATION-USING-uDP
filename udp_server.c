#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 1234

void save_student_info(const char* reg_num, const char* serial_num) {
    FILE* file = fopen("student_info.txt", "a");
    if (file == NULL) {
        perror("Failed to open file");
        exit(1);
    }

    fprintf(file, "Registration Number: %s, Serial Number: %s\n", reg_num, serial_num);
    fclose(file);
}

int main() {
    int server_socket, bytes_read;
    struct sockaddr_in server_address, client_address;
    socklen_t address_len = sizeof(client_address);

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("Failed to create socket");
        exit(1);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Bind failed");
        exit(1);
    }

    printf("Server started. Listening for connections...\n");

    while (1) {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));

        bytes_read = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_address, &address_len);
        if (bytes_read == -1) {
            perror("Failed to receive data");
            exit(1);
        }

        char* token = strtok(buffer, ",");
        char reg_num[100];
        char serial_num[100];

        strncpy(reg_num, token, sizeof(reg_num));
        reg_num[sizeof(reg_num) - 1] = '\0';

        token = strtok(NULL, ",");
        strncpy(serial_num, token, sizeof(serial_num));
        serial_num[sizeof(serial_num) - 1] = '\0';

        save_student_info(reg_num, serial_num);

        sendto(server_socket, "Student information saved successfully.", strlen("Student information saved successfully."), 0, (struct sockaddr*)&client_address, address_len);
    }

    close(server_socket);
    return 0;
}
