// serialToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "serialTool.h"
#include "serialToolDlg.h"
#include "SerialToolDlgFunc.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EPSILON					0.00000001
#define FSPROGRESSMAXRANGE		100

#define COMMSENDDATACYCLE		100//ms
#define SENDCYCLEMAXNUM			3
#define DISPLAYMAXLENGTH		50

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialToolDlg dialog

CSerialToolDlg::CSerialToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSerialToolDlg)
	m_SendStr = _T("");
	m_RecvStr = _T("");
	m_pFileText = NULL;
	m_pFileCurrentIndex = NULL;
	m_HexSendChk = FALSE;
	m_HexRecvChk = FALSE;
	m_bOpenComm = FALSE;
	m_bTimerStart = FALSE;
	m_bRecordRecv = FALSE;
	m_RecvTimestampChk = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSerialToolDlg)
	DDX_Control(pDX, IDC_CHECK_RECVTIMESTAMP, m_RecvTimestampChkBut);
	DDX_Control(pDX, IDC_EDIT_SENDCYCLE, m_SendCycleEdit);
	DDX_Control(pDX, IDC_PROGRESS_SENDFILE, m_ProgressSendFile);
	DDX_Control(pDX, IDC_BUTTON_RECVSAVE, m_RecvSaveBut);
	DDX_Control(pDX, IDC_BUTTON_SETTIMER, m_SetTimerBut);
	DDX_Control(pDX, IDC_EDIT_RECV, m_RecvEdit);
	DDX_Control(pDX, IDC_BUTTON_OPENCOM, m_OpenCommBut);
	DDX_Control(pDX, IDC_BUTTON_OPENSENDFILE, m_OpensendfileBut);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_SendBut);
	DDX_Control(pDX, IDC_CHECK_HEXRECV, m_HexRecvChkBut);
	DDX_Control(pDX, IDC_CHECK_HEXSEND, m_HexSendChkBut);
	DDX_Control(pDX, IDC_EDIT_SEND, m_SendEdit);
	DDX_Control(pDX, IDC_COMBO_PORT, m_PortNumCom);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_BaudRateCom);
	DDX_Control(pDX, IDC_COMBO_STOPBIT, m_StopBitCom);
	DDX_Control(pDX, IDC_COMBO_PARITY, m_ParityCom);
	DDX_Control(pDX, IDC_COMBO_FLOW, m_FLowCtrlCom);
	DDX_Control(pDX, IDC_COMBO_BIT, m_BitWidthCom);
	DDX_Text(pDX, IDC_EDIT_SEND, m_SendStr);
	DDX_Text(pDX, IDC_EDIT_RECV, m_RecvStr);
	DDX_Check(pDX, IDC_CHECK_HEXSEND, m_HexSendChk);
	DDX_Check(pDX, IDC_CHECK_HEXRECV, m_HexRecvChk);
	DDX_Check(pDX, IDC_CHECK_RECVTIMESTAMP, m_RecvTimestampChk);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSerialToolDlg, CDialog)
	//{{AFX_MSG_MAP(CSerialToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPENCOM, OnButtonOpencom)
	ON_CBN_SELCHANGE(IDC_COMBO_PORT, OnSelchangeComboPort)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUD, OnSelchangeComboBaud)
	ON_CBN_SELCHANGE(IDC_COMBO_PARITY, OnSelchangeComboParity)
	ON_CBN_SELCHANGE(IDC_COMBO_STOPBIT, OnSelchangeComboStopbit)
	ON_CBN_SELCHANGE(IDC_COMBO_FLOW, OnSelchangeComboFlow)
	ON_CBN_SELCHANGE(IDC_COMBO_BIT, OnSelchangeComboBit)
	ON_CBN_DROPDOWN(IDC_COMBO_PORT, OnDropdownComboPort)
	ON_BN_CLICKED(IDC_BUTTON_OPENSENDFILE, OnButtonOpensendfile)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_BN_CLICKED(IDC_CHECK_HEXSEND, OnCheckHexsend)
	ON_BN_CLICKED(IDC_CHECK_HEXRECV, OnCheckHexrecv)
	ON_BN_CLICKED(IDC_BUTTON_CLEARRECV, OnButtonClearrecv)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SETTIMER, OnButtonSettimer)
	ON_BN_CLICKED(IDC_BUTTON_RECVSAVE, OnButtonRecvsave)
	ON_BN_CLICKED(IDC_CHECK_RECVTIMESTAMP, OnCheckRecvtimestamp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialToolDlg message handlers

BOOL CSerialToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CString title;						//���ֲ�ͬ����
	title.Format("serialTool-%d", this->m_hWnd);
	SetWindowText(title);

	m_SetTimerBut.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_TIMERSTART));
	m_ProgressSendFile.SetRange(0, FSPROGRESSMAXRANGE);

	m_RecvEdit.SetLimitText(UINT_MAX);

	CStringArray strArrCom;
	CSerialToolDlgFunc::QueryComm(strArrCom);
	for (int i = 0; i < strArrCom.GetSize(); ++i)
	{
		m_PortNumCom.AddString(strArrCom.GetAt(i));
	}
	m_PortNumCom.SetCurSel(2);

	m_BaudRateCom.AddString(_T("115200"));
	m_BaudRateCom.AddString(_T("9600"));
	m_BaudRateCom.AddString(_T("38400"));
	m_BaudRateCom.AddString(_T("57600"));
	m_BaudRateCom.SetCurSel(0);


	m_ParityCom.AddString(_T("N"));
	m_ParityCom.AddString(_T("O"));
	m_ParityCom.AddString(_T("E"));
	m_ParityCom.SetCurSel(0);
	

	m_BitWidthCom.AddString(_T("8"));
	m_BitWidthCom.AddString(_T("7"));
	m_BitWidthCom.SetCurSel(0);

	m_StopBitCom.AddString(_T("1"));
	m_StopBitCom.AddString(_T("2"));
	m_StopBitCom.SetCurSel(0);

	OnSelchangeComboPort();
	OnSelchangeComboBaud(); 
	OnSelchangeComboParity();
	OnSelchangeComboBit(); 
	OnSelchangeComboStopbit();

	m_SendBut.EnableWindow(FALSE);
	m_OpensendfileBut.EnableWindow(FALSE);

	m_SendCycleEdit.SetWindowText("100");
	m_SendCycleEdit.SetLimitText(SENDCYCLEMAXNUM);

	//��ʾ����
	m_tooltip.Create(this);
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_SENDCYCLE), "1~999");
	m_tooltip.Activate(TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSerialToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSerialToolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSerialToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSerialToolDlg::OnButtonOpencom()
{
	// TODO: Add your control notification handler code here
	if(m_rtComm.IsOpened())					//��������Ǵ򿪵ģ����йرմ���
    {
		KillTimer(2);				//�رս�������ʾˢ�µĶ�ʱ��
		((CComboBox *)GetDlgItem(IDC_COMBO_PORT))->EnableWindow(TRUE);			//�����ò���combox
		((CComboBox *)GetDlgItem(IDC_COMBO_BAUD))->EnableWindow(TRUE);
		((CComboBox *)GetDlgItem(IDC_COMBO_PARITY))->EnableWindow(TRUE);
		((CComboBox *)GetDlgItem(IDC_COMBO_BIT))->EnableWindow(TRUE);
		((CComboBox *)GetDlgItem(IDC_COMBO_STOPBIT))->EnableWindow(TRUE);
		
		m_OpensendfileBut.EnableWindow(FALSE);
		m_SendBut.EnableWindow(FALSE);

		m_OpenCommBut.SetWindowText( _T("�򿪴���") );
		m_bOpenComm = FALSE;
		m_bTimerStart = FALSE;

		m_SetTimerBut.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_TIMERSTART));
		KillTimer(1);

		m_rtComm.Close();		//�رմ���

		m_ProgressSendFile.SetPos(0);
		if (m_fileSend.m_pStream)
		{	
			m_fileSend.Close();
			m_OpensendfileBut.SetWindowText("�򿪷����ļ�");
			m_SendEdit.EnableWindow(TRUE);
			m_SendEdit.SetWindowText(NULL);
		}

		if (m_fileRecv.m_pStream)
		{
			m_fileRecv.Close();
			m_bRecordRecv = FALSE;
			m_RecvSaveBut.SetWindowText("��¼������");
		}
    }
	else
	{
		OnSelchangeComboPort();
		SetTimer(2, 200, NULL);		//�򿪽�������ʾˢ�µĶ�ʱ��
		m_PortNum.Delete(0, 3);
		BYTE BParity = NOPARITY;
		if(!m_Parity.Compare("N"))
		{
			BParity = NOPARITY;
		}
		if(!m_Parity.Compare("O"))
		{
			BParity = ODDPARITY;
		}
		if(!m_Parity.Compare("E"))
		{
			BParity = EVENPARITY;
		}
		
		BYTE BStopBit = ONESTOPBIT;
		if(!m_StopBit.Compare("1"))
		{
			BStopBit = ONESTOPBIT;
		}
		if(!m_StopBit.Compare("1.5"))
		{
			BStopBit = ONE5STOPBITS;
		}
		if(!m_StopBit.Compare("2"))
		{
			BStopBit = TWOSTOPBITS;
		}

		m_rtComm.Open(_ttoi(m_PortNum), _ttoi(m_BaudRate), _ttoi(m_BitWidth), BParity, BStopBit);
		m_rtComm.SetRxBufCallBack(&CallBackRecvData, this);

		if (m_rtComm.IsOpened())
		{
			((CComboBox *)GetDlgItem(IDC_COMBO_PORT))->EnableWindow(FALSE);			//�ر����ò���combox
			((CComboBox *)GetDlgItem(IDC_COMBO_BAUD))->EnableWindow(FALSE);
			((CComboBox *)GetDlgItem(IDC_COMBO_PARITY))->EnableWindow(FALSE);
			((CComboBox *)GetDlgItem(IDC_COMBO_BIT))->EnableWindow(FALSE);
			((CComboBox *)GetDlgItem(IDC_COMBO_STOPBIT))->EnableWindow(FALSE);

			m_OpensendfileBut.EnableWindow(TRUE);
			m_SendBut.EnableWindow(TRUE);

			m_OpenCommBut.SetWindowText( _T("�رմ���") );
			m_bOpenComm = TRUE;
		}
		else
		{
			MessageBox("���ڴ�ʧ��!!!");
			return;
		}
	}
}
void CSerialToolDlg::CallBackRecvData(void *pParam, const BYTE * byBuf, DWORD dwLen)
{
	CSerialToolDlg* pCSerialToolDlg = (CSerialToolDlg*)pParam;
	CString strtemp, strPack;

	if(dwLen > 0)					//�¼�ֵΪ2��ʾ���ջ����������ַ�
	{
		strPack.Empty();

		for(DWORD k=0; k<dwLen; k++)
		{
			BYTE bt=*(char*)(byBuf+k);
			if (pCSerialToolDlg->m_HexRecvChk)
			{
				strtemp.Format("%02x ",bt);
			}
			else
			{
				strtemp.Format("%c",bt);
			}

			strPack += strtemp;

			pCSerialToolDlg->m_RecvStrBuff += strtemp;
		}

		if (pCSerialToolDlg->m_bRecordRecv)
		{
			CSerialToolDlgFunc::RecordData(pCSerialToolDlg->m_fileRecv, strPack, pCSerialToolDlg->m_RecvTimestampChk);
		}
		
	}
}

void CSerialToolDlg::OnSelchangeComboPort() 
{
	// TODO: Add your control notification handler code here
	m_PortNumCom.GetLBText( m_PortNumCom.GetCurSel(), m_PortNum);
}

void CSerialToolDlg::OnDropdownComboPort() 
{
	// TODO: Add your control notification handler code here
	m_PortNumCom.SetCurSel(-1);
	m_PortNumCom.ResetContent();

	CStringArray strArrCom;
	CSerialToolDlgFunc::QueryComm(strArrCom);
	for (int i = 0; i < strArrCom.GetSize(); ++i)
	{
		m_PortNumCom.AddString(strArrCom.GetAt(i));
	}
	m_PortNumCom.SetCurSel(1);
}

void CSerialToolDlg::OnSelchangeComboBaud() 
{
	// TODO: Add your control notification handler code here
	m_BaudRateCom.GetLBText( m_BaudRateCom.GetCurSel(), m_BaudRate);
}

void CSerialToolDlg::OnSelchangeComboParity() 
{
	// TODO: Add your control notification handler code here
	m_ParityCom.GetLBText( m_ParityCom.GetCurSel(), m_Parity);
}

void CSerialToolDlg::OnSelchangeComboBit() 
{
	// TODO: Add your control notification handler code here
	m_BitWidthCom.GetLBText( m_BitWidthCom.GetCurSel(), m_BitWidth);
}

void CSerialToolDlg::OnSelchangeComboStopbit() 
{
	// TODO: Add your control notification handler code here
	m_StopBitCom.GetLBText( m_StopBitCom.GetCurSel(), m_StopBit);
}

void CSerialToolDlg::OnSelchangeComboFlow() 
{
	// TODO: Add your control notification handler code here
}


void CSerialToolDlg::OnButtonOpensendfile() 
{
	// TODO: Add your control notification handler code here
    LPCTSTR szFilter = "�ļ� (*.txt)|*.txt|�����ļ� (*.*)|*.*||";
	CString strPathName;
	
	if (m_fileSend.m_pStream)
	{	
		OnButtonSettimer();
		m_fileSend.Close();
		m_OpensendfileBut.SetWindowText("�򿪷����ļ�");
		m_SendEdit.EnableWindow(TRUE);
		m_SendEdit.SetWindowText(NULL);
		delete m_pFileText;
		m_pFileText = NULL;
	}
	else
	{
		CFileDialog fd(TRUE,"*.txt", NULL, OFN_HIDEREADONLY, szFilter);	
		if(IDCANCEL == fd.DoModal())
			return ;
		strPathName = fd.GetPathName();
		
		if(!m_fileSend.Open(strPathName, CFile::shareExclusive | CFile::modeRead))
		{
			AfxMessageBox("��ʧ�ܣ�");
			return ;
		}
		
		//���̹����ļ�·��
		if (strPathName.GetLength() > DISPLAYMAXLENGTH)
		{
			strPathName = strPathName.Left(20) + "..." + strPathName.Right(20);
		}

		m_SendEdit.SetWindowText(strPathName);
		m_SendEdit.EnableWindow(FALSE);

		
		m_ProgressSendFile.SetPos(0);
		m_OpensendfileBut.SetWindowText("�رշ����ļ�");

		m_pFileText = new TCHAR[m_fileSend.GetLength()];
		DWORD fileLen = m_fileSend.GetLength();
		m_readSendFileLength = m_fileSend.Read(m_pFileText, m_fileSend.GetLength());	//!!!CStdioFile��Read������ȥ��'\r'!!!
		m_pFileCurrentIndex = m_pFileText;
		m_pFileText[m_readSendFileLength] = '\0';		//����Ч���ַ����󲹸���β

		m_fileSendcentiLength = m_readSendFileLength / FSPROGRESSMAXRANGE;
		if (0 == m_fileSendcentiLength)
		{
			m_fileSendcentiLength = 1;
		}
		
		{	//�����ļ������̶�ȡһ�����ݣ�׼���÷���״̬
			m_SendPackStr.Empty();
			int ret = 0, retCount = 0;
			while (0 == ret && retCount < 3)		//����3�зǷ��ַ�����!!!������������!!!
			{
				ret = CSerialToolDlgFunc::ProcessingData(m_pFileCurrentIndex, m_SendPackStr);
				++retCount;
			}
		}
	}
}

BOOL CSerialToolDlg::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_RETURN)    
        return TRUE; 
	if (pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_LBUTTONDOWN || 
        pMsg->message == WM_LBUTTONUP)
	{
		m_tooltip.RelayEvent(pMsg);
	}


    return CDialog::PreTranslateMessage(pMsg);
}

void CSerialToolDlg::OnButtonSend() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	BOOL bFLag = m_HexSendChkBut.GetCheck();
	if(bFLag)
	{
		CByteArray hexdata;  
		CSerialToolDlgFunc::ConvertCString2Hex(m_SendStr, hexdata);
		m_rtComm.WriteBuf(hexdata.GetData(), hexdata.GetSize());
	}
	else
	{
		m_rtComm.WriteBuf((BYTE*)m_SendStr.GetBuffer(m_SendStr.GetLength()), m_SendStr.GetLength());
	}
}

void CSerialToolDlg::OnCheckHexsend() 
{
	// TODO: Add your control notification handler code here
	BOOL bFLag = m_HexSendChkBut.GetCheck();
	if(bFLag)
	{
		if(IDCANCEL == MessageBox(_T("16���Ʒ��������Կո�Ϊ�ָ���\neg��7F 89 2A���Ƿ����뽫�����ԣ�"), NULL, MB_OKCANCEL))
		{
			m_HexSendChkBut.SetCheck(BST_UNCHECKED);
			return;
		}
	}

	m_HexSendChkBut.SetCheck(bFLag);
	m_HexSendChk = bFLag;
}

void CSerialToolDlg::OnCheckHexrecv() 
{
	// TODO: Add your control notification handler code here
	BOOL bFLag = m_HexRecvChkBut.GetCheck();
	if(bFLag)
	{}

	m_HexRecvChkBut.SetCheck(bFLag);
	m_HexRecvChk = bFLag;
}

void CSerialToolDlg::OnButtonClearrecv() 
{
	// TODO: Add your control notification handler code here
	m_RecvEdit.Clear();
	m_RecvStr.Empty();
	UpdateData(FALSE);
}

void CSerialToolDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bTimerStart)
	{
		m_bTimerStart = FALSE;
	}
	if(m_rtComm.IsOpened())				//��������Ǵ򿪵ģ����йرմ���
    {
        m_rtComm.Close();
    }
	if (m_fileSend.m_pStream)				//�������ļ��򿪣���ر�
	{
		m_fileSend.Close();
	}
	if (m_fileRecv.m_pStream)
	{
		m_fileRecv.Close();
	}
	if (m_pFileText != NULL)
	{
		delete m_pFileText;
		m_pFileText = NULL;
	}

	CDialog::OnClose();
}

void CSerialToolDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
        case 1:			//UI����
			if (m_fileSend.m_pStream)
			{
				unsigned int posProgress = (m_pFileCurrentIndex - m_pFileText) / m_fileSendcentiLength;
				if (posProgress == m_readSendFileLength)
				{
					posProgress = 100;
				}
				m_ProgressSendFile.SetPos( posProgress );
				if ((m_pFileCurrentIndex - m_pFileText) == (int)m_readSendFileLength)
				{
					OnButtonSettimer();
					KillTimer(1);
					m_fileSend.Close();
					m_OpensendfileBut.SetWindowText("�򿪷����ļ�");
					m_SendEdit.EnableWindow(TRUE);
					m_SendEdit.SetWindowText(NULL);
				}
			}
            break;
        case 2:			//��������ʾˢ�µĶ�ʱ��
			{
				int RecvStrLen = m_RecvEdit.GetWindowTextLength();		
				m_RecvEdit.SetSel(RecvStrLen, RecvStrLen, TRUE);
				m_RecvEdit.ReplaceSel(m_RecvStrBuff);
				m_RecvStrBuff.Empty();
			}
            break;
        case 3:			//���ݰ�����
			{
				//���ݰ��ȷ���ȷ��ʱ�䣬֮���ٶ�ȡ�����Խ����ӳ�
				m_rtComm.WriteBuf((BYTE*)m_SendPackStr.GetBuffer(m_SendPackStr.GetLength()), m_SendPackStr.GetLength());

				m_SendPackStr.Empty();
				int ret = 0, retCount = 0;
				while (0 == ret && retCount < 3)		//����3�зǷ��ַ�����!!!������������!!!
				{
					ret = CSerialToolDlgFunc::ProcessingData(m_pFileCurrentIndex, m_SendPackStr);
					++retCount;
				}
			}
            break;
    }
	CDialog::OnTimer(nIDEvent);
}

void CSerialToolDlg::OnButtonSettimer() 
{
	// TODO: Add your control notification handler code here
	m_bTimerStart = !m_bTimerStart;
	if (m_bTimerStart)
	{
		if (m_fileSend.m_pStream && m_bOpenComm)
		{
			m_SetTimerBut.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_TIMERFIN));
			SetTimer(1, 1000, NULL);

			CString strNum;
			m_SendCycleEdit.GetWindowText(strNum);
			int sendCycle = _ttoi(strNum);
			if (0 == sendCycle)
			{
				sendCycle = 1;
			}
			
			SetTimer(3, sendCycle, NULL);
			m_SendCycleEdit.EnableWindow(FALSE);
		}
	}
	else
	{
			m_SetTimerBut.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_TIMERSTART));
			KillTimer(1);

			KillTimer(3);
			m_SendCycleEdit.EnableWindow(TRUE);
	}
}

void CSerialToolDlg::OnButtonRecvsave() 
{
	// TODO: Add your control notification handler code here
	LPCTSTR szFilter = "�ļ� (*.txt)|*.txt|�����ļ� (*.*)|*.*||";
	CString strPathName;
	CFileDialog fd(FALSE,"*.txt", NULL, OFN_HIDEREADONLY, szFilter);	

	if (m_fileRecv.m_pStream)
	{
		m_fileRecv.Close();
		m_bRecordRecv = FALSE;
		AfxMessageBox("����ɹ���");
		m_RecvSaveBut.SetWindowText("��¼������");
	}
	else
	{
		if(IDCANCEL == fd.DoModal())
			return;
		strPathName = fd.GetPathName();
		
		if(!m_fileRecv.Open(strPathName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		{
			AfxMessageBox("����ʧ�ܣ�");
			m_bRecordRecv = FALSE;
			m_RecvSaveBut.SetWindowText("��¼������");
		}
		else
		{
			m_bRecordRecv = TRUE;
			m_RecvSaveBut.SetWindowText("��¼�С���");
		}
	}
}


void CSerialToolDlg::OnCheckRecvtimestamp() 
{
	// TODO: Add your control notification handler code here
	BOOL bFLag = m_RecvTimestampChkBut.GetCheck();
	if(bFLag)
	{}

	m_RecvTimestampChkBut.SetCheck(bFLag);
	m_RecvTimestampChk = bFLag;	
}