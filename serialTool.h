// serialTool.h : main header file for the SERIALTOOL application
//

#if !defined(AFX_SERIALTOOL_H__FA225409_DFDA_4BA1_AA9B_B757F0EA3E9D__INCLUDED_)
#define AFX_SERIALTOOL_H__FA225409_DFDA_4BA1_AA9B_B757F0EA3E9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSerialToolApp:
// See serialTool.cpp for the implementation of this class
//

class CSerialToolApp : public CWinApp
{
public:
	CSerialToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSerialToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALTOOL_H__FA225409_DFDA_4BA1_AA9B_B757F0EA3E9D__INCLUDED_)
