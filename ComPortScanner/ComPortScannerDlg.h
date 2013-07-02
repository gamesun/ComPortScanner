
// ComPortScannerDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include <vector>

#define WM_NOTIFYICONEVENT		( WM_USER + 100 )

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

	NOTIFYICONDATA m_notify;
	bool m_bIsWindowHiding;
	std::vector<ST_COM_PORT> m_vstComPortInfo;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);
	afx_msg LRESULT OnNotifyIconEvent(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lstCtl_Output;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedHide();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedBtnAbout();
};
