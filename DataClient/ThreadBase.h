#pragma once
class CThreadBase
{
protected:
    BOOL mIsRun;
    HANDLE mhThread;
    BOOL mIsSelfDelete;
public:
    CThreadBase(void);
    virtual ~CThreadBase(void);

    static unsigned int WINAPI ThreadProc(void *pVoid);
    virtual void start();
    virtual void stop();
    virtual void before();
    virtual void after();
    virtual void run() = 0;
};

