#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "tcpclient.h"

using namespace std;

TCPClient::~TCPClient() 
{
    Close();
}


bool TCPClient::Connect() 
{
    // 创建套接字
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockfd == -1) {
        cerr << "无法创建套接字" << endl;
        return false;
    }

    // 设置超时时间为10秒
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
        std::cerr << "Error setting timeout" << std::endl;
		Close();
        return false;
    }
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        std::cerr << "Error setting receive timeout" << std::endl;
		Close();
        return false;
    }
	

    // 设置服务器的地址和端口
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(m_serverPort);
    serverAddress.sin_addr.s_addr = inet_addr(m_server);

    // 连接到服务器
    if (connect(m_sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        cerr << "无法连接到服务器 " <<m_server<<":"<<m_serverPort<<endl;
        Close();
        return false;
    }

    return true;
}

int TCPClient::Send(const char* data, size_t length) 
{
    if (m_sockfd == -1) {
        cerr << "未连接到服务器" << endl;
        return -1;
    }

    return send(m_sockfd, data, length, 0);
}

int TCPClient::Recv(char* buffer, size_t bufferSize) 
{
    if (m_sockfd == -1) 
	{
        cerr << "未连接到服务器" << endl;
        return -1;
    }

    int bytesRead = recv(m_sockfd, buffer, bufferSize, 0);
    if (bytesRead <= 0)
	{
       // cerr << "无法接收服务器数据" << endl;
        return -1;
    }

    buffer[bytesRead] = '\0'; // 确保字符串终止符
    return bytesRead;
}

void  TCPClient::Close()
{
    if (m_sockfd != -1) 
	{
        close(m_sockfd);
        m_sockfd = -1;
    }
}




