#pragma once
#include "ThreadBase.h"
#include "SocketTCP.h"

class CThreadRecv: public CThreadBase
{
    SOCKET mSock;
public:
    CThreadRecv(SOCKET sock)
    {

        mSock = sock;
    }

    virtual void run();

};

class CTheadSend: public CThreadBase
{
public:
    SOCKET mSock;
    CTheadSend(SOCKET sock)
    {
        mSock = sock;
    }
    virtual void run();

};

class CServerTCP : public CThreadBase
{
protected:
    CSocketTCP mSock;
    int mPort;
public:
    CServerTCP(int Port);
    ~CServerTCP(void);
    SOCKET getSock();

    virtual void stop();
    virtual void run();
};



