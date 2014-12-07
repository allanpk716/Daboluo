
// DaboluoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Daboluo.h"
#include "DaboluoDlg.h"
#include "afxdialogex.h"
#include "tlhelp32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND  MY_hWnd;		  //ȫ�ֵİ��ڴ�����
HHOOK m_hkeyboard;    //���̵ײ㹳�Ӿ��
BOOL inCharStatus = FALSE; // �ж��Ƿ�Ϊ����״̬
BOOL bHookOn = FALSE;			//��װȫ�ֹ���Ψһһ��

//����ȫ�ֵĵײ���̹��ӻص�����
LRESULT CALLBACK LowLevelKeyboardProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	if(nCode == HC_ACTION)
	{
		//��ǰ���㴰���ǰ�������ʱ��
		if (MY_hWnd != NULL && GetForegroundWindow() == MY_hWnd)
		{
			//�ж��Ƿ�Ϊ����ģʽ

			if (inCharStatus)
			{
				return CallNextHookEx(m_hkeyboard,nCode,wParam,lParam);
			}

			PKBDLLHOOKSTRUCT kbstruct;
			if (WM_KEYDOWN == wParam)
			{
				kbstruct = (PKBDLLHOOKSTRUCT)lParam;
				//Hook  Space ����
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


// CDaboluoDlg �Ի���




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


// CDaboluoDlg ��Ϣ�������

BOOL CDaboluoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if(::RegisterHotKey(GetSafeHwnd(),IDC_CLIPCURSOR,MOD_CONTROL, VK_END) == FALSE || ::RegisterHotKey(GetSafeHwnd(),IDC_HOTKEY_Nub3,MOD_ALT, 0x33) == FALSE)
	{
		AfxMessageBox("VK_END��ݼ���ռ�����磡");
		bIsRegdit = FALSE;
	}
	else
	{
		bIsRegdit = TRUE;
	}

	InitPrompt();

	MY_hWnd = NULL;
	bIsLock = FALSE;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDaboluoDlg::OnPaint()
{
	if (IsIconic())
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
	//���ϵͳ�����н��̿���
	HANDLE hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hProcessSnap==INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot error");
		return 0;
	}
	//ö���б��еĵ�һ������
	BOOL bProcess=Process32First(hProcessSnap,&pe32);
	while(bProcess)
	{
		//�Ƚ��ҵ��Ľ�����������Ҫ���ҵĽ�������һ���򷵻ؽ���id
		if(strcmp(strupr(pe32.szExeFile),ProcessName)==0)
		{
			MY_hWnd = NULL;
			MY_hWnd = ::FindWindowEx(NULL, MY_hWnd, NULL, "�����Ɖ���III");//�����Ɖ���III
			while(MY_hWnd != NULL)
			{
				DWORD tid = 0;
				GetWindowThreadProcessId(MY_hWnd, &tid);
				DWORD x = GetLastError();
				HANDLE h2 = AfxGetInstanceHandle();
				if(tid == pe32.th32ProcessID)
					break;
				MY_hWnd = ::FindWindowEx(NULL, MY_hWnd, NULL, "�����Ɖ���III");
			}
			CloseHandle(hProcessSnap);
			if(MY_hWnd)
				return TRUE;
			else
				return FALSE;
		}
		//��������
		bProcess=Process32Next(hProcessSnap,&pe32);
	}
	/*CloseHandle(hProcessSnap);*/
	return FALSE;
}

LONG CDaboluoDlg::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	//�����д�Ľ�������֮��������жϴ�Сд�ġ�
	if (bIsProcessExist("DIABLO III.EXE"))//DIABLO III.EXE
	{
		/*	MessageBox("������");*/
		if (!bHookOn)
		{
			if (EnableDebugPrivilege())
			{
				m_hkeyboard = SetWindowsHookEx(WH_KEYBOARD_LL,LowLevelKeyboardProc,GetModuleHandle("Daboluo.exe"),0);

				if (m_hkeyboard == NULL)
					AfxMessageBox("��װ�ͼ����̹���ʧ�ܣ�\n\n ������ʹ�á��ո�ԭ�ع��� ���ܣ�");
				else
					bHookOn = TRUE;
			}
			else
				AfxMessageBox("����Ȩ��ʧ�ܣ�\n\n ������ʹ�á��ո�ԭ�ع��� ���ܣ�");
		}
	}
	else
	{
		MessageBox("����û�п���");
		return 0;
	}

	/*	"�����Ɖ���III"*/
	if(MY_hWnd == NULL)
	{
		bIsLock = FALSE;
		bHookOn = FALSE;
		return 0;
	}

	// ��������
	if(wParam == IDC_CLIPCURSOR)
	{
		bIsLock = !bIsLock;
	}

	// �����Ƿ�ʹ�� ѭ�� ���� 3
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
	// ��������ͼ��
	m_Nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_Nid.hWnd = m_hWnd;
	m_Nid.uID = IDR_MAINFRAME;
	m_Nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	m_Nid.uCallbackMessage = WM_SHOWTASK; // �Զ������̲˵���Ϣ
	m_Nid.hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	strcpy(m_Nid.szTip, ("����3�������"));
	Shell_NotifyIcon(NIM_ADD, &m_Nid); // �����������ͼ��
}

LRESULT CDaboluoDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	if(wParam != IDR_MAINFRAME) // �ж�ͼ��ID�Ƿ����
		return 1;
	switch(lParam)
	{
	case WM_RBUTTONUP: 
		{// ������Ҽ�
			CPoint point;
			GetCursorPos(&point);
			CMenu menu;
			menu.CreatePopupMenu();
			menu.AppendMenu(MF_STRING, IDM_SHOW_WINDOW, ("��ʾ"));
			menu.AppendMenu(MF_STRING, WM_DESTROY, _T("�˳�"));
			SetForegroundWindow();  // ����˵�ʧȥ���㲻��ʧ��BUG
			menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
			menu.DestroyMenu(); 
		}
		break;
	case WM_LBUTTONDBLCLK: // ˫��������
		OnShowWindow();
		break;
	}
	return 0;
}

// ���̲˵�"��ʾ"��Ӧ����
void CDaboluoDlg::OnShowWindow()
{
	if(IsWindowVisible())
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, (LPARAM)m_hWnd);
	else
		ShowWindow(SW_SHOW);
}

LRESULT CDaboluoDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{

	// ������С����ť
	if (message == WM_SYSCOMMAND && 
		wParam == SC_MINIMIZE)
	{
		//AfxMessageBox(_T("�Ƿ���С�����ڵ�����"));
		ShowWindow(SW_HIDE); // ����������
		return 0;
	}
	else if (message == WM_SYSCOMMAND && 
		wParam == SC_CLOSE)
	{
		//AfxMessageBox(_T("�Ƿ���С�����ڵ�����"));
		ShowWindow(SW_HIDE); // ����������
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
	Shell_NotifyIcon(NIM_DELETE, &m_Nid); // ��������ɾ��ͼ��

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