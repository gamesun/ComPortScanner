
// ComPortScannerDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include <WinUser.h>
#include <Windows.h>
#include <SetupAPI.h>
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Setupapi.lib")
#include <vector>
#include "Dbt.h"
#include <DEVGUID.H>

typedef struct _comport{
	CString strPortName;
	bool bIsOldPort;
	bool bIsExistPort;
} ST_COM_PORT;

// CComPortScannerDlg dialog
class CComPortScannerDlg : public CDialogEx
{
// Construction
public:
	CComPortScannerDlg(CWnd* pParent = NULL);	// standard constructor

	void ShowHideWindow( void );

	long DeviceEnumComPort( std::vector<ST_COM_PORT>& );
	void UpdatePortInfo( bool );

// Dialog Data
	enum { IDD = IDD_COMPORTSCANNER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	std::vector<ST_COM_PORT> m_vstComPortInfo;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lstCtl_Output;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedHide();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
