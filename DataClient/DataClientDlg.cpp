
// DataClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataClient.h"
#include "DataClientDlg.h"
#include "afxdialogex.h"
#include "ServerTCP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // 对话框数据
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    // 实现
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDataClientDlg 对话框



CDataClientDlg::CDataClientDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CDataClientDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


BEGIN_MESSAGE_MAP(CDataClientDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_SERVER, &CDataClientDlg::OnBnClickedBtnServer)
    ON_BN_CLICKED(IDC_BTN_SEND, &CDataClientDlg::OnBnClickedBtnSend)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDataClientDlg 消息处理程序

BOOL CDataClientDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if(pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if(!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);         // 设置大图标
    SetIcon(m_hIcon, FALSE);        // 设置小图标

    //初始化服务器IP和端口到界面编辑框中
    SetDlgItemText(IDC_EDIT_SERVER_IP, TEXT("127.0.0.1")); //IP
    SetDlgItemInt(IDC_EDIT_SERVER_PORT, 8888); //端口

    SetDlgItemText(IDC_EDIT_SEND, TEXT("45.875")); //初始化发送编辑框数值.



    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDataClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDataClientDlg::OnPaint()
{
    if(IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDataClientDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


//连接/断开服务器
void CDataClientDlg::OnBnClickedBtnServer()
{
    GetDlgItem(IDC_BTN_SERVER)->EnableWindow(FALSE); //禁用按钮, 避免连续多次点击

    CString tStr;
    GetDlgItemText(IDC_BTN_SERVER, tStr); //获取按钮文本
    if(tStr == TEXT("连接")) //如果是连接,则进行连接操作
    {
        CString strServerIP;
        GetDlgItemText(IDC_EDIT_SERVER_IP, strServerIP); //获取界面上输入的IP
        int serverPort = GetDlgItemInt(IDC_EDIT_SERVER_PORT); //获取界面上输入的端口

        //启动模拟服务器
        //////////////////////////////////////////////////////////////////////////
        //正式版本需要删除掉
        CServerTCP *pServer = new CServerTCP(serverPort); //创建模拟服务器封装类
        mServerSock = pServer->getSock(); //保存其socket
        pServer->start(); //启动线程
        Sleep(300); //sleep是为了让服务器先完全启动起来, 方便后面连接
        //////////////////////////////////////////////////////////////////////////


        //客户端
        mClient.setListener(this); //设置事件监听器
        mClient.setServer(strServerIP, serverPort); //设置服务器IP和端口
        mClient.start(); //启动数据接收线程
    }
    else //否则是断开操作
    {
        mClient.stop();//断开连接

        //停止模拟服务器
        //////////////////////////////////////////////////////////////////////////
        //正式版本需要删除掉
        CSocketTCP sock(mServerSock);
        sock.close();
        //////////////////////////////////////////////////////////////////////////
    }



}


void CDataClientDlg::OnBnClickedBtnSend()
{
    CString tStr;
    GetDlgItemText(IDC_BTN_SERVER, tStr);
    if(tStr == TEXT("连接"))//说明还未连接服务器,不能发送
    {
        MessageBox(TEXT("服务器未连接"));
        return;
    }


    GetDlgItemText(IDC_EDIT_SEND, tStr); //获取编辑框中输入的发送的数据
    float val = atof(tStr);
    char buf[0x10] = {0};
    *(float*)buf = val;
    buf[4] = ',';
    mClient.send(buf, 5); //发送数据
}


//连接事件
void CDataClientDlg::onConnect(BOOL bSuccess)
{
    GetDlgItem(IDC_BTN_SERVER)->EnableWindow(TRUE); //让按钮可点击(前面禁用了)
    SetDlgItemText(IDC_BTN_SERVER, bSuccess ? "断开" : "连接"); //根据状态, 设置按钮文字
}

//数据事件
void CDataClientDlg::onRecv(const char *pBuf, int len)
{
    //收到数据, 解析三个符点数
    float val1 = *(float*)pBuf;
    float val2 = *(float*)&pBuf[5];
    float val3 = *(float*)&pBuf[10];

    //分别把三个符点数显示到编辑框中.
    CString tStr;
    tStr.Format(TEXT("%.02f"), val1);
    SetDlgItemText(IDC_EDIT_VAL1, tStr);

    tStr.Format(TEXT("%.02f"), val2);
    SetDlgItemText(IDC_EDIT_VAL2, tStr);

    tStr.Format(TEXT("%.02f"), val3);
    SetDlgItemText(IDC_EDIT_VAL3, tStr);
}

//断开连接事件
void CDataClientDlg::onDisconnect()
{
    SetDlgItemText(IDC_BTN_SERVER, "连接");
    GetDlgItem(IDC_BTN_SERVER)->EnableWindow(TRUE);

    //////////////////////////////////////////////////////////////////////////
    //正式版本需要删除掉
    CSocketTCP sock(mServerSock);
    sock.close();
    //////////////////////////////////////////////////////////////////////////

}

void CDataClientDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

void CDataClientDlg::OnDestroy()
{
    __super::OnDestroy();

    mClient.stop();

    //////////////////////////////////////////////////////////////////////////
    //正式版本需要删除掉
    CSocketTCP sock(mServerSock);
    sock.close();
    //////////////////////////////////////////////////////////////////////////
}
