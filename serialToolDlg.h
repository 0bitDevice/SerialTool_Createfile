// serialToolDlg.h : header file
//
//{{AFX_INCLUDES()
#include "RT_COM.h"
//}}AFX_INCLUDES

#include <afxmt.h>

#if !defined(AFX_SERIALTOOLDLG_H__DF70D52F_48A3_417D_BBCA_BDFD6858B126__INCLUDED_)
#define AFX_SERIALTOOLDLG_H__DF70D52F_48A3_417D_BBCA_BDFD6858B126__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSerialToolDlg dialog

class CSerialToolDlg : public CDialog
{
// Construction
public:
	CSerialToolDlg(CWnd* pParent = NULL);	// standard constructor
	BOOL PreTranslateMessage(MSG* pMsg);
	static void CallBackRecvData(void *pParam, const BYTE * byBuf, DWORD dwLen);
// Dialog Data
	//{{AFX_DATA(CSerialToolDlg)
	enum { IDD = IDD_SERIALTOOL_DIALOG };
	CButton	m_RecvTimestampChkBut;
	CEdit	m_SendCycleEdit;
	CProgressCtrl	m_ProgressSendFile;
	CButton	m_RecvSaveBut;
	CButton	m_SetTimerBut;
	CEdit	m_RecvEdit;
	CButton	m_OpenCommBut;
	CButton	m_OpensendfileBut;
	CButton	m_SendBut;
	CButton	m_HexRecvChkBut;
	CButton	m_HexSendChkBut;
	CEdit	m_SendEdit;
	CComboBox	m_PortNumCom;
	CComboBox	m_BaudRateCom;
	CComboBox	m_StopBitCom;
	CComboBox	m_ParityCom;
	CComboBox	m_FLowCtrlCom;
	CComboBox	m_BitWidthCom;
	CString		m_PortNum;
	CString		m_BaudRate;
	CString		m_StopBit;
	CString		m_Parity;
	CString		m_FLowCtrl;
	CString		m_BitWidth;
	CString		m_SendStr;
	CString		m_SendPackStr;
	CString		m_RecvStr;
	TCHAR*		m_pFileText;
	TCHAR*		m_pFileCurrentIndex;
	BOOL	m_HexSendChk;
	BOOL	m_HexRecvChk;
	BOOL	m_RecvTimestampChk;
	//}}AFX_DATA

	RT_COM		m_rtComm;
	CStdioFile	m_fileSend;
	DWORD		m_fileSendcentiLength;
	DWORD		m_readSendFileLength;
	CStdioFile	m_fileRecv;
	CString		m_RecvStrBuff;
	BOOL		m_bRecordRecv;
	BOOL		m_bOpenComm;
	BOOL		m_bTimerStart;
	CToolTipCtrl m_tooltip;
	TCHAR		m_SendPackStrArr[1000];
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialToolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSerialToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonOpencom();
	afx_msg void OnSelchangeComboPort();
	afx_msg void OnSelchangeComboBaud();
	afx_msg void OnSelchangeComboParity();
	afx_msg void OnSelchangeComboStopbit();
	afx_msg void OnSelchangeComboFlow();
	afx_msg void OnSelchangeComboBit();
	afx_msg void OnDropdownComboPort();
	afx_msg void OnButtonOpensendfile();
	afx_msg void OnButtonSend();
	afx_msg void OnCheckHexsend();
	afx_msg void OnCheckHexrecv();
	afx_msg void OnButtonClearrecv();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonSettimer();
	afx_msg void OnButtonRecvsave();
	afx_msg void OnCheckRecvtimestamp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALTOOLDLG_H__DF70D52F_48A3_417D_BBCA_BDFD6858B126__INCLUDED_)
