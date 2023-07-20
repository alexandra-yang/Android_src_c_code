
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


int main() {
    int sockfd;
    struct sockaddr_un addr;
    char message[] = "Hello from TX!";

    printf("TX is up!");

    // Get the app's private data directory path from the environment variable
    const char *appPrivateDir = getenv("APP_PRIVATE_DIR");
    if (appPrivateDir == NULL) {
        fprintf(stderr, "APP_PRIVATE_DIR environment variable not set.\n");
        return 1;
    }

    // Create the socket file path
    char socketFilePath[128];
    snprintf(socketFilePath, sizeof(socketFilePath), "%s/my_socket", appPrivateDir);


    while (1) {
        sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sockfd == -1) {
            perror("socket");
            return 1;
        }


        memset(&addr, 0, sizeof(struct sockaddr_un));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, socketFilePath, sizeof(addr.sun_path) - 1);

        if (connect(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) == -1) {
            perror("connect");
            close(sockfd);
            return 1;
        }

        if (send(sockfd, message, strlen(message) + 1, 0) == -1) {
            perror("send");
        } else {
            printf("TX: Sent message to RX\n");
        }

        close(sockfd);

        sleep(5);
    }

    return 0;
}

