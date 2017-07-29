
// DataClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataClient.h"
#include "DataClientDlg.h"
#include "afxdialogex.h"
#include "ServerTCP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // �Ի�������
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    // ʵ��
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


// CDataClientDlg �Ի���



CDataClientDlg::CDataClientDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CDataClientDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


BEGIN_MESSAGE_MAP(CDataClientDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //    ON_BN_CLICKED(IDC_BTN_SERVER, &CDataClientDlg::OnBnClickedBtnServer)
    ON_BN_CLICKED(IDC_BTN_SEND, &CDataClientDlg::OnBnClickedBtnSend)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDataClientDlg ��Ϣ�������

BOOL CDataClientDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // ��������...���˵�����ӵ�ϵͳ�˵��С�

    // IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

    // ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon(m_hIcon, TRUE);         // ���ô�ͼ��
    SetIcon(m_hIcon, FALSE);        // ����Сͼ��

    SetDlgItemText(IDC_EDIT_SEND, TEXT("45.875")); //��ʼ�����ͱ༭����ֵ.


    //����ģ�������
    //////////////////////////////////////////////////////////////////////////
    /*
    //��ʽ�汾��Ҫɾ����
    CServerTCP *pServer = new CServerTCP(8888); //����ģ���������װ��
    mServerSock = pServer->getSock(); //������socket
    pServer->start(); //�����߳�
    Sleep(300); //sleep��Ϊ���÷���������ȫ��������, �����������
    //*/
    //////////////////////////////////////////////////////////////////////////

    //�ͻ���
    mClient.setListener(this); //�����¼�������
    mClient.setServer(TEXT("127.0.0.1"), 8888); //���÷�����IP�Ͷ˿�
    mClient.start(); //�������ݽ����߳�



    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDataClientDlg::OnPaint()
{
    if(IsIconic())
    {
        CPaintDC dc(this); // ���ڻ��Ƶ��豸������

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // ʹͼ���ڹ����������о���
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // ����ͼ��
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDataClientDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}




void CDataClientDlg::OnBnClickedBtnSend()
{
    CString tStr;
    GetDlgItemText(IDC_EDIT_SEND, tStr); //��ȡ�༭��������ķ��͵�����
    float val = atof(tStr);
    char buf[0x10] = {0};
    *(float*)buf = val;
    buf[4] = ',';
    mClient.send(buf, 5); //��������
}


//�����¼�
void CDataClientDlg::onConnect(BOOL bSuccess)
{
    if(bSuccess == FALSE)
    {
        MessageBox(TEXT("���ӷ�����ʧ��!!! �����Զ��˳�"));
        PostMessage(WM_CLOSE);
    }
}

//�����¼�
void CDataClientDlg::onRecv(const char *pBuf, int len)
{
    //�յ�����, ��������������
    float val1 = *(float*)pBuf;
    float val2 = *(float*)&pBuf[5];
    float val3 = *(float*)&pBuf[10];

    //�ֱ��������������ʾ���༭����.
    CString tStr;
    tStr.Format(TEXT("%.02f"), val1);
    SetDlgItemText(IDC_EDIT_VAL1, tStr);

    tStr.Format(TEXT("%.02f"), val2);
    SetDlgItemText(IDC_EDIT_VAL2, tStr);

    tStr.Format(TEXT("%.02f"), val3);
    SetDlgItemText(IDC_EDIT_VAL3, tStr);
}

//�Ͽ������¼�
void CDataClientDlg::onDisconnect()
{

    /* //////////////////////////////////////////////////////////////////////////
     //��ʽ�汾��Ҫɾ����
     CSocketTCP sock(mServerSock);
     sock.close();
     //////////////////////////////////////////////////////////////////////////*/

    MessageBox(TEXT("�����Ѿ��Ͽ�!!!!! �����Զ��˳�"));
    PostMessage(WM_CLOSE);

}

void CDataClientDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

void CDataClientDlg::OnDestroy()
{
    __super::OnDestroy();

    mClient.stop();

    /*//////////////////////////////////////////////////////////////////////////
    //��ʽ�汾��Ҫɾ����
    CSocketTCP sock(mServerSock);
    sock.close();
    //////////////////////////////////////////////////////////////////////////*/
}
