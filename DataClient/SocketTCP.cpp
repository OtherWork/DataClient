#include "stdafx.h"
#include "SocketTCP.h"


CSocketTCP::CSocketTCP(void)
{
    mSock = INVALID_SOCKET;
    create();
}


CSocketTCP::CSocketTCP(SOCKET sock, BOOL bConnect/*=FALSE*/)
{
    mSock = sock;
    mIsConnect = bConnect;
}

CSocketTCP::~CSocketTCP(void)
{
}

int CSocketTCP::setOpt(int optName,  const void*optValue, int optSize)
{
    return ::setsockopt(mSock, SOL_SOCKET, optName, (const char*)optValue, optSize);
}

SOCKET CSocketTCP::create()
{
    close();
    mSock = socket(AF_INET, SOCK_STREAM, 0);
    mIsConnect = FALSE;
    return mSock;
}

int CSocketTCP::setRecvTimeout(int ms)
{
    return setOpt(SO_RCVTIMEO, &ms, sizeof(int));
}

int CSocketTCP::setSendTimeout(int ms)
{
    return setOpt(SO_SNDTIMEO, &ms, sizeof(int));
}

bool CSocketTCP::bind(unsigned short port)
{
    SOCKADDR_IN  addr = {0};
    addr.sin_addr.S_un.S_addr = ADDR_ANY;
    addr.sin_port = htons(port);
    addr.sin_family  = AF_INET;
    return ::bind(mSock, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN)) == 0;
}

unsigned short CSocketTCP::randBind()
{
    unsigned short port = 0;
    srand(GetTickCount() + * (int*)this);
    int tryTime = 10;
    do
    {
        unsigned short port = rand() % 50000 + 10000;
        if(bind(port))
        {
            break;
        }
        --tryTime;
    }
    while(tryTime > 0);
    return port;
}


void CSocketTCP::close()
{
    if(mSock == INVALID_SOCKET)
    {
        return;
    }
    ::shutdown(mSock, SD_BOTH);
    ::closesocket(mSock);
    mSock = INVALID_SOCKET;
    mIsConnect = FALSE;
}

SOCKET CSocketTCP::getSock()
{
    return mSock;
}

void CSocketTCP::setNoneBlock(BOOL enable)
{
    unsigned long iMode = enable ? 1 : 0;
    ioctlsocket(mSock, FIONBIO, &iMode);
}

BOOL CSocketTCP::isConnected()
{
    return mIsConnect;
}

bool CSocketTCP::connect(const char*ip, unsigned short port, int timeoutMS)
{
    SOCKADDR_IN addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    setNoneBlock(TRUE);
    if(SOCKET_ERROR  ==  ::connect(mSock, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN)))
    {
        fd_set fdset;
        FD_ZERO(&fdset);
        struct timeval  tv;
        FD_SET(mSock, &fdset);
        tv.tv_sec = timeoutMS / 1000;
        tv.tv_usec = timeoutMS % 1000;

        int error = -1;

        //select >0,表示有信号, 否则就是超时了.
        if(select(mSock + 1, NULL, &fdset, NULL, &tv) > 0)
        {
            int len = sizeof(int);
            //判断是否有错误发生
            getsockopt(mSock, SOL_SOCKET, SO_ERROR, (char *)&error, /*(socklen_t *)*/&len);
        }

        if(error != 0)
        {
            close();
            return false;
        }
    }
    setNoneBlock(FALSE);
    mIsConnect = TRUE;
    return true;
}

int CSocketTCP::send(const void* pBuf, int len)
{
    return ::send(mSock, (const char*)pBuf, len, 0);
}

int CSocketTCP::recv(void *pBuf, int size)
{
    int ret =  ::recv(mSock, (char*)pBuf, size, 0);
    if(ret > 1 && ret < size)
    {
        ((char*)pBuf)[ret] = 0;
    }
    return ret;
}

void CSocketTCP::listen(int backLog)
{
    ::listen(mSock, backLog);
}

SOCKET CSocketTCP::accept(SOCKADDR_IN *addr)
{
    int nameLen = sizeof(SOCKADDR_IN);
    return ::accept(mSock, (SOCKADDR*)addr, &nameLen);
}

SOCKET CSocketTCP::accept()
{
    SOCKADDR_IN addr = {0};
    return accept(&addr);
}
