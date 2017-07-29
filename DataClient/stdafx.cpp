
// stdafx.cpp : 只包括标准包含文件的源文件
// DataClient.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


#pragma comment(lib,"ws2_32")


void printLog(LPCTSTR lpszFmt, ...)
{
    va_list args;
    va_start(args, lpszFmt);
    int len = _vsctprintf(lpszFmt, args) + 4;
    len = min(1024 * 10, len);
    TCHAR *lpszBuf = (TCHAR*)_alloca(len * sizeof(TCHAR)); //栈中分配, 不需要释放
    len = _vstprintf_s(lpszBuf, len , lpszFmt, args);
    va_end(args);
    if(lpszBuf[len - 1] != '\n')
    {
        lpszBuf[len] = '\r';
        lpszBuf[len + 1] = '\n';
        lpszBuf[len + 2] = 0;
    }
    OutputDebugString(lpszBuf);
}