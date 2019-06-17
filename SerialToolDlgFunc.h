// SerialToolDlgFunc.h: interface for the CSerialToolDlgFunc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALTOOLDLGFUNC_H__CB10B155_F5E3_434B_92AF_5D05DEE57507__INCLUDED_)
#define AFX_SERIALTOOLDLGFUNC_H__CB10B155_F5E3_434B_92AF_5D05DEE57507__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSerialToolDlgFunc  
{
public:
	CSerialToolDlgFunc();
	virtual ~CSerialToolDlgFunc();

	static void QueryComm(CStringArray&);  
	static int ConvertHex2CString(CString hexStr, CString dstStr);
	static char ConvertHexChar(char ch);
	static int ConvertCString2Hex(CString str, CByteArray& senddata);
	static int ProcessingData(CStdioFile& file, CString& strData);
	static int ProcessingData(TCHAR* &pFileText, CString& strData);
	static int RecordData(CStdioFile& file, CString& strData, BOOL bRecTime = FALSE);
};

#endif // !defined(AFX_SERIALTOOLDLGFUNC_H__CB10B155_F5E3_434B_92AF_5D05DEE57507__INCLUDED_)
