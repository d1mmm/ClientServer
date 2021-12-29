#include <iostream>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

const int PORT = 8888;

void server()
{
    int server;
    struct sockaddr_in address;
    int addr_len = sizeof(address);
    int client;

    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == 0)
    {
        std::cout << "Socket failed\n";
        exit(1);
    }

    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    address.sin_family = AF_INET;

    if (bind(server, (struct sockaddr *)&address, addr_len) != 0)
    {
        std::cout << "Socket bind failed\n";
        exit(1);
    }

    std::cout << "Server started listening\n";
    if (listen(server, 1) != 0)
    {
        std::cout << "Error listening\n";
    }

    while (true)
    {
        char buf[1024] = {0};
        client = accept(server, (struct sockaddr *)&address, (socklen_t *)&addr_len);
        if (client == 0)
        {
            std::cout << "Server connection failed\n";
            exit(1);
        }

        int read_len = read(client, buf, 1024);
        std::cout << "Message from the client: " << buf << std::endl;
        const char *mess = buf;
        if (strcmp(mess, "hello") == 0)
        {
            mess = "world";
        }
        send(client, mess, strlen(mess), 0);
    }
}

void client(char *str)
{
    int client;
    sockaddr_in address;
    int addr_len = sizeof(address);

    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client == 0)
    {
        std::cout << "Socket failed\n";
        exit(1);
    }

    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    address.sin_family = AF_INET;
        char buff[1024] = {0};
        if (connect(client, (struct sockaddr *)&address, addr_len))
        {
            std::cout << "Client connection failed\n";
            exit(1);
        }
        send(client, str, strlen(str), 0);
        int read_len = read(client, buff, 1024);
        std::cout << "Message from the server: " << buff << std::endl;
}

int main(int argc, char *argv[])
{
    char *str = new char;
    if (argc < 2)
    {
        std::cout << "Few arguments\n";
        exit(1);
    }
    if (strcmp(argv[1], "client") == 0)
    {
        std::cout << "Enter Message -> ";
        std::cin.getline(str, 256);
        client(str);
    }
    if (strcmp(argv[1], "server") == 0)
    {
        server();
    }
    delete str;
}