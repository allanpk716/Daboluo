
// DaboluoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Daboluo.h"
#include "DaboluoDlg.h"
#include "afxdialogex.h"
#include "tlhelp32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND  MY_hWnd;		  //全局的暗黑窗体句柄
HHOOK m_hkeyboard;    //键盘底层钩子句柄
BOOL inCharStatus = FALSE; // 判断是否为聊天状态
BOOL bHookOn = FALSE;			//安装全局钩子唯一一次

//定义全局的底层键盘钩子回调函数
LRESULT CALLBACK LowLevelKeyboardProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	if(nCode == HC_ACTION)
	{
		//当前顶层窗体是暗黑三的时候
		if (MY_hWnd != NULL && GetForegroundWindow() == MY_hWnd)
		{
			//判断是否为聊天模式

			if (inCharStatus)
			{
				return CallNextHookEx(m_hkeyboard,nCode,wParam,lParam);
			}

			PKBDLLHOOKSTRUCT kbstruct;
			if (WM_KEYDOWN == wParam)
			{
				kbstruct = (PKBDLLHOOKSTRUCT)lParam;
				//Hook  Space 按键
				if (VK_SPACE == kbstruct->vkCode)
				{
					keybd_event(VK_LSHIFT,0,0,0);
					//	Sleep(30);
					mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
					Sleep(30);
					mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
					//	Sleep(30);
					keybd_event(VK_LSHIFT,0,KEYEVENTF_KEYUP,0);
					return 1;
				}	
			}
		}
	}

	return CallNextHookEx(m_hkeyboard,nCode,wParam,lParam);
}

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


// CDaboluoDlg 对话框




CDaboluoDlg::CDaboluoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDaboluoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDaboluoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDaboluoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_TIMER()

	ON_MESSAGE(WM_HOTKEY,OnHotKey)
	ON_MESSAGE(WM_SHOWTASK, OnShowTask)
	ON_COMMAND(IDM_SHOW_WINDOW, OnShowWindow)
END_MESSAGE_MAP()


// CDaboluoDlg 消息处理程序

BOOL CDaboluoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	if(::RegisterHotKey(GetSafeHwnd(),IDC_CLIPCURSOR,MOD_CONTROL, VK_END) == FALSE || ::RegisterHotKey(GetSafeHwnd(),IDC_HOTKEY_Nub3,MOD_ALT, 0x33) == FALSE)
	{
		AfxMessageBox("VK_END快捷键被占用了噻！");
		bIsRegdit = FALSE;
	}
	else
	{
		bIsRegdit = TRUE;
	}

	InitPrompt();

	MY_hWnd = NULL;
	bIsLock = FALSE;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDaboluoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
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

void CDaboluoDlg::OnPaint()
{
	if (IsIconic())
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
HCURSOR CDaboluoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDaboluoDlg::OnClose() 
{
	CDialog::OnClose();
}

void CDaboluoDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent ==1 && MY_hWnd != NULL && GetForegroundWindow()->GetSafeHwnd() == MY_hWnd)
	{
		CRect Cr;
		CPoint Crl, Crr;
		::GetClientRect(MY_hWnd, &Cr);
		Crl.x = Cr.left;
		Crl.y = Cr.top;
		Crr.x = Cr.right;
		Crr.y = Cr.bottom;
		::ClientToScreen(MY_hWnd, &Crl);
		::ClientToScreen(MY_hWnd, &Crr);
		Cr.left = Crl.x;
		Cr.top = Crl.y;
		Cr.right = Crr.x;
		Cr.bottom = Crr.y;
		::ClipCursor(&Cr);
	}

	CButton *pButton = (CButton *)GetDlgItem(IDC_CHECK_Nub3);
	BOOL checked = pButton->GetCheck();

	if(nIDEvent ==2 && MY_hWnd != NULL && GetForegroundWindow()->GetSafeHwnd() == MY_hWnd && checked)
	{
		keybd_event(0x33,0,0,0);
		Sleep(30);
		keybd_event(0x33,0,KEYEVENTF_KEYUP,0);
	}


	CDialog::OnTimer(nIDEvent);
}

BOOL   CDaboluoDlg::bIsProcessExist(char *ProcessName)
{
	PROCESSENTRY32 pe32;
	pe32.dwSize=sizeof(pe32);
	//获得系统内所有进程快照
	HANDLE hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hProcessSnap==INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot error");
		return 0;
	}
	//枚举列表中的第一个进程
	BOOL bProcess=Process32First(hProcessSnap,&pe32);
	while(bProcess)
	{
		//比较找到的进程名和我们要查找的进程名，一样则返回进程id
		if(strcmp(strupr(pe32.szExeFile),ProcessName)==0)
		{
			MY_hWnd = NULL;
			MY_hWnd = ::FindWindowEx(NULL, MY_hWnd, NULL, "暗黑破壞神III");//暗黑破壞神III
			while(MY_hWnd != NULL)
			{
				DWORD tid = 0;
				GetWindowThreadProcessId(MY_hWnd, &tid);
				DWORD x = GetLastError();
				HANDLE h2 = AfxGetInstanceHandle();
				if(tid == pe32.th32ProcessID)
					break;
				MY_hWnd = ::FindWindowEx(NULL, MY_hWnd, NULL, "暗黑破壞神III");
			}
			CloseHandle(hProcessSnap);
			if(MY_hWnd)
				return TRUE;
			else
				return FALSE;
		}
		//继续查找
		bProcess=Process32Next(hProcessSnap,&pe32);
	}
	/*CloseHandle(hProcessSnap);*/
	return FALSE;
}

LONG CDaboluoDlg::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	//必须大写的进程名，之后会智能判断大小写的。
	if (bIsProcessExist("DIABLO III.EXE"))//DIABLO III.EXE
	{
		/*	MessageBox("大菠萝在");*/
		if (!bHookOn)
		{
			if (EnableDebugPrivilege())
			{
				m_hkeyboard = SetWindowsHookEx(WH_KEYBOARD_LL,LowLevelKeyboardProc,GetModuleHandle("Daboluo.exe"),0);

				if (m_hkeyboard == NULL)
					AfxMessageBox("安装低级键盘钩子失败！\n\n 将不能使用‘空格’原地攻击 功能！");
				else
					bHookOn = TRUE;
			}
			else
				AfxMessageBox("提升权限失败！\n\n 将不能使用‘空格’原地攻击 功能！");
		}
	}
	else
	{
		MessageBox("大菠萝没有开启");
		return 0;
	}

	/*	"暗黑破壞神III"*/
	if(MY_hWnd == NULL)
	{
		bIsLock = FALSE;
		bHookOn = FALSE;
		return 0;
	}

	// 设置锁定
	if(wParam == IDC_CLIPCURSOR)
	{
		bIsLock = !bIsLock;
	}

	// 设置是否使用 循环 数字 3
	if (wParam == IDC_HOTKEY_Nub3)
	{
		CButton *pButton = (CButton *)GetDlgItem(IDC_CHECK_Nub3);
		BOOL checked = pButton->GetCheck();

		pButton->SetCheck(!checked);
	}

	if(bIsLock)
	{
		SetTimer(1, 200, NULL);
		SetTimer(2, 200, NULL);
	}
	else
	{
		KillTimer(2);
		KillTimer(1);
		::ClipCursor(NULL);
		bHookOn = FALSE;
		if (m_hkeyboard != NULL)
		{
			UnhookWindowsHookEx(m_hkeyboard);
		}
	}

	return 0;
}

BOOL CDaboluoDlg::EnableDebugPrivilege() 
{ 
	HANDLE hToken; 
	BOOL fOk=FALSE; 
	if(OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken)) 
	{ 
		TOKEN_PRIVILEGES tp; 
		tp.PrivilegeCount=1; 
		if(!LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tp.Privileges[0].Luid)) 
			AfxMessageBox("Can't lookup privilege value.\n"); 
		tp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED; 
		if(!AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(tp),NULL,NULL)) 
			AfxMessageBox("Can't adjust privilege value.\n"); 
		fOk=(GetLastError()==ERROR_SUCCESS); 
		CloseHandle(hToken); 
	} 
	return fOk; 
} 

void CDaboluoDlg::InitPrompt(void)
{
	// 设置托盘图标
	m_Nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_Nid.hWnd = m_hWnd;
	m_Nid.uID = IDR_MAINFRAME;
	m_Nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	m_Nid.uCallbackMessage = WM_SHOWTASK; // 自定义托盘菜单消息
	m_Nid.hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	strcpy(m_Nid.szTip, ("大菠萝3锁定鼠标"));
	Shell_NotifyIcon(NIM_ADD, &m_Nid); // 在托盘区添加图标
}

LRESULT CDaboluoDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	if(wParam != IDR_MAINFRAME) // 判断图标ID是否相符
		return 1;
	switch(lParam)
	{
	case WM_RBUTTONUP: 
		{// 按鼠标右键
			CPoint point;
			GetCursorPos(&point);
			CMenu menu;
			menu.CreatePopupMenu();
			menu.AppendMenu(MF_STRING, IDM_SHOW_WINDOW, ("显示"));
			menu.AppendMenu(MF_STRING, WM_DESTROY, _T("退出"));
			SetForegroundWindow();  // 解决菜单失去焦点不消失的BUG
			menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
			menu.DestroyMenu(); 
		}
		break;
	case WM_LBUTTONDBLCLK: // 双击鼠标左键
		OnShowWindow();
		break;
	}
	return 0;
}

// 托盘菜单"显示"响应函数
void CDaboluoDlg::OnShowWindow()
{
	if(IsWindowVisible())
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, (LPARAM)m_hWnd);
	else
		ShowWindow(SW_SHOW);
}

LRESULT CDaboluoDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{

	// 单击最小化按钮
	if (message == WM_SYSCOMMAND && 
		wParam == SC_MINIMIZE)
	{
		//AfxMessageBox(_T("是否最小化窗口到托盘"));
		ShowWindow(SW_HIDE); // 隐藏主窗口
		return 0;
	}
	else if (message == WM_SYSCOMMAND && 
		wParam == SC_CLOSE)
	{
		//AfxMessageBox(_T("是否最小化窗口到托盘"));
		ShowWindow(SW_HIDE); // 隐藏主窗口
		return 0;
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}

HWND CDaboluoDlg::GetSysTrayWnd()
{

	HWND hwnd ;
	hwnd = ::FindWindow("Shell_TrayWnd", NULL ) ;

	hwnd = ::FindWindowEx(hwnd, 0, "TrayNotifyWnd", NULL );
	return hwnd ;
}

BOOL CDaboluoDlg::DestroyWindow() 
{
	if (bIsRegdit)
	{
		UnregisterHotKey(GetSafeHwnd(), IDC_CLIPCURSOR);
		UnregisterHotKey(GetSafeHwnd(), IDC_HOTKEY_Nub3);
	}
	Shell_NotifyIcon(NIM_DELETE, &m_Nid); // 在托盘区删除图标

	KillTimer(1);
	KillTimer(2);

	if (m_hkeyboard != NULL)
	{
		UnhookWindowsHookEx(m_hkeyboard);
	}

	HWND hwnd = GetSysTrayWnd()  ;
	::InvalidateRect(hwnd,   NULL,   TRUE);
	//	RefurbishTray();
	Sleep(20);

	return CDialog::DestroyWindow();
}

BOOL CDaboluoDlg::PreTranslateMessage(MSG* pMsg) 
{ 
	return CDialog::PreTranslateMessage(pMsg);
}