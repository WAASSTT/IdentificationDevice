
// IdentificationDeviceDlg.h: 头文件
//

#pragma once
extern "C" {
#include "hidsdi.h"
#include <setupapi.h>
#include "dbt.h"
}

#pragma comment(lib,"hid.lib")
#pragma comment(lib,"setupapi.lib")


#include<vector>
#include<iostream>
// CIdentificationDeviceDlg 对话框
class CIdentificationDeviceDlg : public CDialogEx
{
	// 构造
public:
	CIdentificationDeviceDlg(CWnd* pParent = nullptr);	// 标准构造函数

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IDENTIFICATIONDEVICE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	// 实现
protected:
	HICON m_hIcon;
	//===========打开设备相关 begin=====================================================
	int ProductID = 0x0000;//825
	int VendorID = 0x30FA;
	CString USBString = L"";

	GUID								HidGuid;//hid设备Guid集合
	HANDLE								hDevInfo;//设备信息集句柄
	PSP_DEVICE_INTERFACE_DETAIL_DATA	detailData;//设备详细信息

	HANDLE								DeviceHandle;//设备句柄

	BOOL 								WinDevString = FALSE;//判断设备字符串是否匹配
	BOOL								MyDeviceDetected = FALSE;//是否接入设备
	HIDP_CAPS							Capabilities;//设备功能
	HIDP_CAPS							rdCapabilities;//检测按键功能
	HIDP_CAPS							wrCapabilities;//写入EEPROM功能
	HANDLE								wrOPReportHandle;
	HANDLE								rdIPReportHandle;

	std::vector<USHORT> pid;
	std::vector<USHORT> vid;
	std::vector<CString> DevStr;

	//===========打开设备相关 end=====================================================
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);
	void RegisterForDeviceNotifications(HWND m_HWND);
	void GetHidGuid();
	BOOL GetDeviceCapabilities();
	BOOL OpenDevice();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit2;
	CEdit m_edit1;
};
