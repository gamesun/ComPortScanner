
// ComPortScannerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ComPortScanner.h"
#include "ComPortScannerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CComPortScannerDlg dialog




CComPortScannerDlg::CComPortScannerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CComPortScannerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CComPortScannerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstCtl_Output);
}

BEGIN_MESSAGE_MAP(CComPortScannerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CComPortScannerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CComPortScannerDlg::OnBnClickedHide)
	ON_BN_CLICKED(IDCANCEL, &CComPortScannerDlg::OnBnClickedCancel)
	ON_WM_ACTIVATE()
	ON_WM_DEVICECHANGE()
END_MESSAGE_MAP()


// CComPortScannerDlg message handlers

BOOL CComPortScannerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CComPortScannerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CComPortScannerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CComPortScannerDlg::OnBnClickedOk()
{
	UpdatePortInfo( false );
//	CDialogEx::OnOK();
}

void CComPortScannerDlg::UpdatePortInfo( bool bIsRemove )
{
	std::vector<ST_COM_PORT> vstComPortInfoNew;
	
	m_lstCtl_Output.DeleteAllItems();

	DeviceEnumComPort( vstComPortInfoNew );

	for ( std::vector<ST_COM_PORT>::iterator n = vstComPortInfoNew.begin(); n != vstComPortInfoNew.end(); n++ ){
		for ( std::vector<ST_COM_PORT>::iterator o = m_vstComPortInfo.begin(); o != m_vstComPortInfo.end(); o++ ){
			if ( n->strPortName == o->strPortName ){
				n->bIsOldPort = true;
				o->bIsExistPort = true;
			}
		}

		if ( n->bIsOldPort ){
		// old
			m_lstCtl_Output.InsertItem( distance( vstComPortInfoNew.begin(), n ), n->strPortName );
		} else {
		// new
			m_lstCtl_Output.InsertItem( distance( vstComPortInfoNew.begin(), n ), "Add " + n->strPortName );
		}
	}

	if ( bIsRemove ){
		for ( std::vector<ST_COM_PORT>::iterator o = m_vstComPortInfo.begin(); o != m_vstComPortInfo.end(); o++ ){
			if ( ! o->bIsExistPort ){
			// remove
				m_lstCtl_Output.InsertItem( distance( m_vstComPortInfo.begin(), o ), "Remove " + o->strPortName );
			}
		}
	}

	m_vstComPortInfo = vstComPortInfoNew;
}


void CComPortScannerDlg::OnBnClickedHide()
{

}


void CComPortScannerDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


void CComPortScannerDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);


}

BOOL CComPortScannerDlg::OnDeviceChange(UINT nEventType, DWORD dwData)
{
	DEV_BROADCAST_DEVICEINTERFACE* dbd;
	void *p = (void*)dwData;
	dbd = (DEV_BROADCAST_DEVICEINTERFACE*) p;

	switch (nEventType){
	case DBT_DEVICEREMOVECOMPLETE:
		UpdatePortInfo( true );
		break;
	case DBT_DEVICEARRIVAL:
		UpdatePortInfo( false );
		break;
	default:
		break;
	}

	return TRUE;
}

long CComPortScannerDlg::DeviceEnumComPort( std::vector<ST_COM_PORT> &vstComPort )
{
	HDEVINFO hDevInfo;
	SP_DEVINFO_DATA DeviceInfoData;
	DWORD DeviceIndex;
	
/*
  GUID_DEVCLASS_FDC
  GUID_DEVCLASS_DISPLAY
  GUID_DEVCLASS_CDROM
  GUID_DEVCLASS_KEYBOARD
  GUID_DEVCLASS_COMPUTER
  GUID_DEVCLASS_SYSTEM
  GUID_DEVCLASS_DISKDRIVE
  GUID_DEVCLASS_MEDIA
  GUID_DEVCLASS_MODEM
  GUID_DEVCLASS_MOUSE
  GUID_DEVCLASS_NET
  GUID_DEVCLASS_USB
  GUID_DEVCLASS_FLOPPYDISK
  GUID_DEVCLASS_UNKNOWN
  GUID_DEVCLASS_SCSIADAPTER
  GUID_DEVCLASS_HDC
  GUID_DEVCLASS_PORTS
  GUID_DEVCLASS_MONITOR
*/
	// Create a HDECINFO with all present devices.
	hDevInfo = SetupDiGetClassDevs( (LPGUID)&GUID_DEVCLASS_PORTS, 0, 0, DIGCF_PRESENT );

	if ( hDevInfo == INVALID_HANDLE_VALUE ){
		DWORD ErrorCode = GetLastError();
		// Insert error handling here.
		return -1;
	}

	// Enumerate through all devices in Set.
	DeviceInfoData.cbSize = sizeof( SP_DEVINFO_DATA );

	for ( DeviceIndex = 0; SetupDiEnumDeviceInfo( hDevInfo, DeviceIndex, &DeviceInfoData ); DeviceIndex++ ){
		DWORD DataPropertyType;
		LPTSTR buffer = NULL;
		DWORD buffersize = 0;

		while ( !SetupDiGetDeviceRegistryProperty(
			hDevInfo,
			&DeviceInfoData,
			SPDRP_FRIENDLYNAME,
			&DataPropertyType,
			(PBYTE)buffer,
			buffersize,
			&buffersize ) ){
			DWORD ErrorCode = GetLastError();
			if ( ErrorCode == ERROR_INSUFFICIENT_BUFFER ){
				// Change the buffer size.
				if ( buffer ) LocalFree( buffer );
				buffer = (LPTSTR)LocalAlloc( LPTR, buffersize );
			} else {
				// Insert error handling here.
				break;
			}
		}

		if ( buffer == NULL ){
			continue;
		}
		//CString FriendlyPortName = buffer;
		int nStart, nEnd;
		ST_COM_PORT stComPort;
		stComPort.bIsOldPort = false;
		stComPort.bIsExistPort = false;
		stComPort.strPortName.Format( buffer );
		nStart = stComPort.strPortName.ReverseFind( '(' ) + 1;
		nEnd = stComPort.strPortName.ReverseFind( ')' );
		stComPort.strPortName = stComPort.strPortName.Mid( nStart, nEnd - nStart ) + "  " + stComPort.strPortName;

		vstComPort.push_back( stComPort );

		if ( buffer ){
			LocalFree( buffer );
		}
	}

	if ( GetLastError() != NO_ERROR
		&& GetLastError() != ERROR_NO_MORE_ITEMS ){
		return -1;
	}
	// Cleanup.
	SetupDiDestroyDeviceInfoList( hDevInfo );

	return 0;
}
