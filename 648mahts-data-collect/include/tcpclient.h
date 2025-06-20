#ifndef TCPCLIENT_H
#define TCPCLIENT_H

class TCPClient {
public:
    TCPClient(const char* server, int serverPort)
        : m_server(server), m_serverPort(serverPort), m_sockfd(-1) {}

    ~TCPClient();

    bool Connect();

    int Send(const char* data, size_t length);

    int Recv(char* buffer, size_t bufferSize);

    void Close();


private:
    const char* m_server;
    int m_serverPort;
    int m_sockfd;
};
#endif

