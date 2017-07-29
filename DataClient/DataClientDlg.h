
// DataClientDlg.h : ͷ�ļ�
//

#pragma once
#include "ClientTCP.h"


// CDataClientDlg �Ի���
class CDataClientDlg : public CDialogEx, public OnClientListener
{
    // ����
public:
    CDataClientDlg(CWnd* pParent = NULL);   // ��׼���캯��

    // �Ի�������
    enum { IDD = IDD_DATACLIENT_DIALOG };

    //ʵ�������¼�����
    virtual void onConnect(BOOL bSuccess);

    //ʵ�����ݽ����¼�����
    virtual void onRecv(const char *pBuf, int len);

    //ʵ�ֶϿ��¼�����
    virtual void onDisconnect();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    CClientTCP mClient;   //�ͻ��˷�װ��

    SOCKET mServerSock; //ģ�������(��λ��).----��ʽ�汾����ɾ����

    // ʵ��
protected:
    HICON m_hIcon;

    // ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    //    afx_msg void OnBnClickedBtnServer(); //����/�Ͽ���������ť��Ӧ����
    afx_msg void OnBnClickedBtnSend(); //������Ӧ����
    afx_msg void OnDestroy(); //�Ի�������
};
