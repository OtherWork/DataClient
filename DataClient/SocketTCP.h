#pragma once
class CSocketTCP
{
protected:
    SOCKET mSock;
    BOOL mIsConnect;
public:
    CSocketTCP(void);
    CSocketTCP(SOCKET sock, BOOL bConnect = FALSE);
    ~CSocketTCP(void);
    SOCKET create();
    int setRecvTimeout(int ms);
    int setOpt(int optName, const void*optValue, int optSize);
    int setSendTimeout(int ms);
    bool bind(unsigned short port);
    unsigned short randBind();
    void close();
    SOCKET getSock();
    void setNoneBlock(BOOL enable);
    BOOL isConnected();


    bool connect(const char*ip, unsigned short port, int timeoutMS);
    int send(const void* pBuf, int len);
    int recv(void *pBuf, int size);
    void listen(int backLog = 100);
    SOCKET accept();
    SOCKET accept(SOCKADDR_IN *addr);
};

