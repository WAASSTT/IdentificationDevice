
// IdentificationDeviceDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "IdentificationDevice.h"
#include "IdentificationDeviceDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CIdentificationDeviceDlg 对话框



CIdentificationDeviceDlg::CIdentificationDeviceDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IDENTIFICATIONDEVICE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIdentificationDeviceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
}

BEGIN_MESSAGE_MAP(CIdentificationDeviceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_DEVICECHANGE()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CIdentificationDeviceDlg 消息处理程序

BOOL CIdentificationDeviceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetHidGuid();
	RegisterForDeviceNotifications(m_hWnd);
	OpenDevice();
	
	CString str,str2, devStr,devStr2;
	int x = 0;
	for (auto i : pid)
	{
		str.Format(L"pid = %hx,vid = %hx\r\n", pid[x], vid[x]);
		str2 += str;
		devStr.Format(L"%s\r\n", DevStr[x]);
		devStr2 += devStr;
		x++;
	}
	m_edit2.SetWindowTextW(str2);
	m_edit1.SetWindowTextW(devStr2);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CIdentificationDeviceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CIdentificationDeviceDlg::OnPaint()
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
HCURSOR CIdentificationDeviceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CIdentificationDeviceDlg::OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
{
	switch (nEventType)
	{
	case DBT_DEVICEREMOVECOMPLETE: {
		pid.clear();
		vid.clear();
		DevStr.clear();
		OpenDevice();
		CString str, str2, devStr, devStr2;
		int x = 0;
		for (auto i : pid)
		{
			str.Format(L"pid = %hx,vid = %hx\r\n", pid[x], vid[x]);
			str2 += str;
			devStr.Format(L"%s\r\n", DevStr[x]);
			devStr2 += devStr;
			x++;
		}
		m_edit2.SetWindowTextW(str2);
		m_edit1.SetWindowTextW(devStr2);
		UpdateData(FALSE);
		return TRUE; 
	}
	case DBT_DEVICEARRIVAL: {
		pid.clear();
		vid.clear();
		DevStr.clear();
		OpenDevice();
		CString str, str2, devStr, devStr2;
		int x = 0;
		for (auto i : pid)
		{
			str.Format(L"pid = %hx,vid = %hx\r\n", pid[x], vid[x]);
			str2 += str;
			devStr.Format(L"%s\r\n", DevStr[x]);
			devStr2 += devStr;
			x++;
		}
		m_edit2.SetWindowTextW(str2);
		m_edit1.SetWindowTextW(devStr2);
		UpdateData(FALSE);
		break; 
	}
	}
	return TRUE;
}


void CIdentificationDeviceDlg::RegisterForDeviceNotifications(HWND m_HWND)  //跟usb插拔有关
{
	DEV_BROADCAST_DEVICEINTERFACE DevBroadcastDeviceInterface;
	HDEVNOTIFY DeviceNotificationHandle;

	DevBroadcastDeviceInterface.dbcc_size = sizeof(DevBroadcastDeviceInterface);  //设置结 构体大小
	DevBroadcastDeviceInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;   //设备类型
	DevBroadcastDeviceInterface.dbcc_classguid = HidGuid;   //设备接口GUID
	DeviceNotificationHandle = RegisterDeviceNotification(m_HWND, &DevBroadcastDeviceInterface, DEVICE_NOTIFY_WINDOW_HANDLE);   //注册设备接口
}

void CIdentificationDeviceDlg::GetHidGuid()
{
		HidD_GetHidGuid(&HidGuid);  //得到系统HID设备结构数组指针
}

//获取设备功能
BOOL CIdentificationDeviceDlg::GetDeviceCapabilities()
{
	PHIDP_PREPARSED_DATA	PreparsedData;
	if (HidD_GetPreparsedData(DeviceHandle, &PreparsedData)) {
		HidP_GetCaps(PreparsedData, &Capabilities);
		HidD_FreePreparsedData(PreparsedData);
		return TRUE;
	}
	return FALSE;
}


BOOL CIdentificationDeviceDlg::OpenDevice()
{
	HIDD_ATTRIBUTES                Attributes;//设备属性
	SP_DEVICE_INTERFACE_DATA       devInfoData;//检索详细信息的接口
	int                            MemberIndex = 0;
	LONG                           Result;
	ULONG						   Length = 0;
	ULONG						   Required;
	detailData = NULL;
	DeviceHandle = NULL;

	HidD_GetHidGuid(&HidGuid);  //得到系统HID设备结构数组指针

	hDevInfo = SetupDiGetClassDevs
	(&HidGuid,
		NULL,
		NULL,
		DIGCF_PRESENT | DIGCF_INTERFACEDEVICE
	);
	devInfoData.cbSize = sizeof(devInfoData);

	MemberIndex = 0;

	int x = 0;
	while (x < 100)
	{
		x++;

		Result = SetupDiEnumDeviceInterfaces
		(hDevInfo,
			0,
			&HidGuid,
			MemberIndex,
			&devInfoData);

		if (Result != 0)
		{
			Result = SetupDiGetDeviceInterfaceDetail
			(hDevInfo,
				&devInfoData,
				NULL,
				0,
				&Length,
				NULL);
			detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(Length);

			detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			Result = SetupDiGetDeviceInterfaceDetail
			(hDevInfo,
				&devInfoData,
				detailData,
				Length,
				&Required,
				NULL);

			DeviceHandle = CreateFile
			(detailData->DevicePath,
				0,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				(LPSECURITY_ATTRIBUTES)NULL,
				OPEN_EXISTING,
				0,
				NULL);

			Attributes.Size = sizeof(Attributes);

			Result = HidD_GetAttributes
			(DeviceHandle,
				&Attributes);

			MyDeviceDetected = FALSE;

			char buff[100];
			memset(buff, 0, 100);
			HidD_GetProductString(DeviceHandle, buff, 100);
			CString devStr;//从设备中读出的字符串
			devStr.Format(_T("%s"), buff);


			MyDeviceDetected = FALSE;
			BOOL isExist = FALSE;
			if (pid.empty() && vid.empty())
			{
				isExist = TRUE;
			}
			else {
				if (DevStr.empty()) {
					devStr = L"NULL";
				}
				else {
					int i;
					for (i = 0; i < (int)pid.size(); i++)
					{
						if (pid[i] == Attributes.ProductID&&vid[i] == Attributes.VendorID && !DevStr[i].Compare(devStr))
						{
							break;
						}
					}
					if (i == pid.size())
					{
						isExist = TRUE;
					}
				}
			}
			if (isExist) {
				pid.push_back(Attributes.ProductID);
				vid.push_back(Attributes.VendorID);
				DevStr.push_back(devStr);
			}
			free(detailData);
		}

		MemberIndex = MemberIndex + 1;
	}

	SetupDiDestroyDeviceInfoList(hDevInfo);
	return MyDeviceDetected;
}
