
// DataClientDlg.h : 头文件
//

#pragma once
#include "ClientTCP.h"


// CDataClientDlg 对话框
class CDataClientDlg : public CDialogEx, public OnClientListener
{
    // 构造
public:
    CDataClientDlg(CWnd* pParent = NULL);   // 标准构造函数

    // 对话框数据
    enum { IDD = IDD_DATACLIENT_DIALOG };

    //实现连接事件函数
    virtual void onConnect(BOOL bSuccess);

    //实现数据接收事件函数
    virtual void onRecv(const char *pBuf, int len);

    //实现断开事件函数
    virtual void onDisconnect();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    CClientTCP mClient;   //客户端封装类

    SOCKET mServerSock; //模拟服务器(下位机).----正式版本需在删除掉

    // 实现
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    //    afx_msg void OnBnClickedBtnServer(); //连接/断开服务器按钮响应函数
    afx_msg void OnBnClickedBtnSend(); //发送响应函数
    afx_msg void OnDestroy(); //对话框销毁
};
