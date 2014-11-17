
// DaboluoDlg.h : ͷ�ļ�
//

#pragma once


// CDaboluoDlg �Ի���
class CDaboluoDlg : public CDialogEx
{
// ����
public:
	CDaboluoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DABOLUO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

// ʵ��
protected:
	HICON m_hIcon;
	BOOL bIsLock;//�Ƿ�����
	BOOL bIsRegdit;//�Ƿ�ע��ɹ�
	BOOL   bIsProcessExist(char *ProcessName);
	BOOL EnableDebugPrivilege();//��ȡ����Ȩ��

	void InitPrompt(void);
	NOTIFYICONDATA m_Nid;//��С��������
	HWND GetSysTrayWnd();//ˢ��ϵͳ����ͼ��

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);

	afx_msg LONG OnHotKey(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShowWindow();
	DECLARE_MESSAGE_MAP()
};
