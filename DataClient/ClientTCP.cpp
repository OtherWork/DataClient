#include "stdafx.h"
#include "ClientTCP.h"


CClientTCP::CClientTCP(void)
{
    mpListener = nullptr;
    mServerPort = 0;
    mIsSelfDelete = FALSE;
}


CClientTCP::~CClientTCP(void)
{
}

void CClientTCP::setListener(OnClientListener *pListener)
{
    mpListener = pListener;
}

void CClientTCP::setServer(LPCTSTR strServerIP, int serverPort)
{
    mServerIP = strServerIP;
    mServerPort = serverPort;
}

void CClientTCP::stop()
{
    __super::stop();
    mSock.close(); //释放tcp socket
    mSock.create();
}

void CClientTCP::after()
{
    mSock.close();  //释放tcp socket
    mSock.create();
}

void CClientTCP::run()
{
    printLog("client begin recv thread\n");
    //连接服务器
    if(mSock.connect(mServerIP, mServerPort, 5000) == false)
    {
        printLog("连接失败\n");
        if(mpListener)
        {
            mpListener->onConnect(FALSE); //连接失败, 发出失败事件通知
        }
        return;
    }
    printLog("连接成功\n");
    if(mpListener)
    {
        mpListener->onConnect(TRUE); //发出连接成功事件
    }

    int ret = 0;
    char buf[0x100] = {0};
    while(mIsRun)
    {
        ret = mSock.recv(buf, 14); //每次接收14字节
        if(ret < 1) //失败, 结束接收
        {
            printLog("接收失败\n");
            break;
        }
        if(mpListener)
        {
            mpListener->onRecv(buf, ret); //接收成功, 发出数据接收事件.
        }
    }

    printLog("断开连接\n");
    if(mpListener)
    {
        mpListener->onDisconnect();//发出断开连接事件.
    }
}

int CClientTCP::send(const void *pBuf, int len)
{
    return mSock.send(pBuf, len);//向服务端发送数据
}
