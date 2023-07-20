#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/data/local/tmp/my_socket"

int main() {
    int sockfd, client_sockfd;
    struct sockaddr_un addr, client_addr;
    socklen_t client_len;
    char buffer[100];

    printf("RX is up!\n");


    // Get the app's private data directory path from the environment variable
    const char *appPrivateDir = getenv("APP_PRIVATE_DIR");
    if (appPrivateDir == NULL) {
        printf("APP_PRIVATE_DIR environment variable not set.\n");
        return 1;
    }
    printf("APP_PRIVATE_DIR environment= %s.\n", appPrivateDir);

    // Create the socket file path
    char socketFilePath[128];
    snprintf(socketFilePath, sizeof(socketFilePath), "%s/my_socket", appPrivateDir);

    printf(" RX socket= %s.\n", socketFilePath);


    // Start socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Create socket failed");
        return 1;
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketFilePath, sizeof(addr.sun_path) - 1);

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) == -1) {
        printf("Bind socket failed\n");
        perror("Bind");
        close(sockfd);
        return 1;
    }

    if (listen(sockfd, 1) == -1) {
        perror("Listen socket failed\n");
        perror("Listen");
        close(sockfd);
        return 1;
    }

    printf("RX: Waiting for connection...\n");
    while (1) {
        printf("RX: Waiting for connection...\n");

        client_len = sizeof(client_addr);
        client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
        if (client_sockfd == -1) {
            printf("Accept msg from socket failed");
            close(sockfd);
            return 1;
        }

        int bytesRead = recv(client_sockfd, buffer, sizeof(buffer), 0);
        if (bytesRead == -1) {
            printf("Recv msg from socket failed");
        } else {
            printf("RX: Received message from TX: %s\n", buffer);
        }

        close(client_sockfd);
        sleep(5);
    }

    close(sockfd);

    printf("RX is off Q_Q !");
    return 0;
}
