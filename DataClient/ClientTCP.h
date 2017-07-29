#pragma once
#include "ThreadBase.h"
#include "SocketTCP.h"

//事件监听器
class OnClientListener
{
public:
    virtual void onConnect(BOOL bSuccess) = 0; //连接事件
    virtual void onRecv(const char *pBuf, int len) = 0; //数据接收事件
    virtual void onDisconnect() = 0; //连接断开事件
};

//客户端工作类
class CClientTCP: public CThreadBase
{
protected:
    CString mServerIP; //服务器IP
    int mServerPort; //服务器端口
    CSocketTCP mSock; //tcp socket包装类
    OnClientListener *mpListener; //事件监听器指针
public:
    CClientTCP(void);
    ~CClientTCP(void);
    void setListener(OnClientListener *pListener); //设置事件监听器指针
    void setServer(LPCTSTR strServerIP, int serverPort); //设置服务器信息

    //停止/断开与服务器连接
    virtual void stop();

    //断开后的清理工作
    virtual void after();

    //线程中的工作任务
    virtual void run();

    //向服务器发送数据的函数
    int send(const void *pBuf, int len);

};

