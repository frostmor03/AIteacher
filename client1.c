#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd)
{
    char buff[MAX];
    uint32_t number; // Для демонстрации пересылки целых чисел

    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter a number to send: ");
        scanf("%u", &number);
        getchar(); // Считываем '\n' после ввода

        // Преобразуем число в Big Endian
        // uint32_t number_to_send = htonl(number);
        // memcpy(buff, &number_to_send, sizeof(number_to_send));

        // Отправляем данные серверу
        write(sockfd, buff, sizeof(uint32_t));

        bzero(buff, sizeof(buff));

        // Читаем ответ от сервера
        read(sockfd, buff, sizeof(buff));

        // Преобразуем число из Big Endian
        memcpy(&number, buff, sizeof(number));
        // number = ntohl(number);

        printf("From server: Received number: %u\n", number);

        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;

    // Создаем сокет
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    } else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // Настраиваем IP и порт
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.148.37");
    servaddr.sin_port = htons(PORT);

    // Подключаемся к серверу
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection with the server failed...\n");
        exit(0);
    } else
        printf("Connected to the server..\n");

    // Общение с сервером
    func(sockfd);

    // Закрываем сокет
    close(sockfd);
}
