#pragma once
#include "stdafx.h"
class CSocketAutoInit
{
public:
    CSocketAutoInit()
    {
        WORD wVersionRequested;
        WSADATA wsaData;
        int err;

        wVersionRequested = MAKEWORD(2, 2);

        err = WSAStartup(wVersionRequested, &wsaData);

    }
    ~CSocketAutoInit()
    {
        WSACleanup();
    }
};


