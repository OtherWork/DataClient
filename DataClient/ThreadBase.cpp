#include "stdafx.h"
#include "ThreadBase.h"


CThreadBase::CThreadBase(void)
{
    mIsRun = FALSE;
    mhThread = NULL;
    mIsSelfDelete = TRUE;
}


CThreadBase::~CThreadBase(void)
{
}

unsigned int WINAPI CThreadBase::ThreadProc(void *pVoid)
{
    CThreadBase *pThread = (CThreadBase*)pVoid;
    pThread->before();
    pThread->run();
    pThread->after();
    if(pThread->mIsSelfDelete)
    {
        delete pThread;
    }
    return 0;
}

void CThreadBase::start()
{
    mIsRun = TRUE;
    mhThread = (HANDLE)  _beginthreadex(NULL, 0, ThreadProc, this, 0, NULL);
}

void CThreadBase::stop()
{
    mIsRun = FALSE;
    if(mhThread)
    {
        CloseHandle(mhThread);
        mhThread = NULL;
    }
}

void CThreadBase::before()
{

}

void CThreadBase::after()
{

}
