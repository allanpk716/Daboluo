
// DaboluoDlg.h : 头文件
//

#pragma once


// CDaboluoDlg 对话框
class CDaboluoDlg : public CDialogEx
{
// 构造
public:
	CDaboluoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DABOLUO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

// 实现
protected:
	HICON m_hIcon;
	BOOL bIsLock;//是否锁定
	BOOL bIsRegdit;//是否注册成功
	BOOL   bIsProcessExist(char *ProcessName);
	BOOL EnableDebugPrivilege();//获取调试权限

	void InitPrompt(void);
	NOTIFYICONDATA m_Nid;//最小化任务栏
	HWND GetSysTrayWnd();//刷新系统托盘图标

	// 生成的消息映射函数
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
