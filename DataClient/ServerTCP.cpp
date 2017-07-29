#include "stdafx.h"
#include "ServerTCP.h"


CServerTCP::CServerTCP(int port)
{
    mPort = port;
}


CServerTCP::~CServerTCP(void)
{
}

SOCKET CServerTCP::getSock()
{
    return mSock.getSock();
}

void CServerTCP::stop()
{
    __super::stop();
    mSock.close();
}

void CServerTCP::run()
{
    printLog("server begin accept thread\n");
    int ret = mSock.bind(mPort);
    if(ret < 1)
    {
        printLog("bind error:%d\n", WSAGetLastError());
        return;
    }
    mSock.listen();
    SOCKADDR_IN addr = {0};
    SOCKET clientSock = INVALID_SOCKET;
    while((clientSock = mSock.accept(&addr)) != INVALID_SOCKET)
    {
        CThreadRecv *pThread = new CThreadRecv(clientSock);
        pThread->start();
        CTheadSend *pSend = new CTheadSend(clientSock);
        pSend->start();

    }
    printLog("server end accept thread\n");
}

void CThreadRecv::run()
{
    printLog("beign server recv thread\n");
    CSocketTCP sock(mSock);
    char buf[0x100] = {0};
    while(mIsRun)
    {
        int ret = sock.recv(buf, 0x100);
        if(ret < 1)
        {
            break;
        }
        printLog("server recv:%.02f\n", *(float*)buf);
    }
    printLog("end server recv thread\n");
}


void CTheadSend::run()
{
    printLog("begin auto send....\n");
    int ret = 0;
    CSocketTCP sock(mSock);
    char buf[0x10] = {0};
    srand(GetTickCount());
    while(mIsRun)
    {
        *(float*)&buf[0] = rand();
        buf[4] = ',';
        *(float*)&buf[5] = rand();
        buf[9] = ',';
        *(float*)&buf[10] = rand();
        ret = sock.send(buf, 14);
        if(ret < 1)
        {
            break;
        }
        Sleep(1000);
    }
    printLog("end auto send\n");
}
