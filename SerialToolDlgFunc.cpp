// SerialToolDlgFunc.cpp: implementation of the CSerialToolDlgFunc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "serialTool.h"
#include "SerialToolDlgFunc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	COMCOUNT				16

#define TIMEDEVIATION			10//ms
#define RECVDATACYCLE			100//ms


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerialToolDlgFunc::CSerialToolDlgFunc()
{

}

CSerialToolDlgFunc::~CSerialToolDlgFunc()
{

}
void CSerialToolDlgFunc::QueryComm(CStringArray& strArrCom)
{
	HANDLE hCom;
	CString strComName,strCom;

	for (int i = 1; i <= COMCOUNT; ++i)
	{
		strComName.Format("\\\\.\\COM%d", i);
		hCom = CreateFile(strComName, GENERIC_READ | GENERIC_WRITE, 0, 0,  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
		if(INVALID_HANDLE_VALUE != hCom )
		{
			//能打开该串口，则添加该串口
			strCom.Format("COM%d", i);
			strArrCom.Add(strCom);

			CloseHandle(hCom);
		}
	}
}

int CSerialToolDlgFunc::ConvertHex2CString(CString hexStr, CString dstStr)
{
	return -1;
}

char CSerialToolDlgFunc::ConvertHexChar(char ch)  
{  
	if((ch>='0')&&(ch<='9')) 
		return ch-0x30;  
	else if((ch>='A')&&(ch<='F')) 
		return ch-'A'+10;  
	else if((ch>='a')&&(ch<='f')) 
		return ch-'a'+10;
	else return (-1); 
}

int CSerialToolDlgFunc::ConvertCString2Hex(CString SrcStr,CByteArray &senddata)
{
	int hexData,lowhexData;
	int hexDataLen = 0;
	int SrcStrLen = SrcStr.GetLength();
	
	if(1 == SrcStrLen%3)
	{
		if (' ' != SrcStr.GetAt(SrcStrLen-1))	//防止输入字符时的疏忽：最后一个字符不是空格
		{
			SrcStr.SetAt(SrcStrLen-1, ' ');
		}
		senddata.SetSize((SrcStrLen+1)/2);			
	}
	else
	{
		senddata.SetSize(SrcStrLen/2);			
	}

	for(int i = 0;i<SrcStrLen;)
	{
		if(i >= SrcStrLen)
			break;
		
		char hstr = SrcStr[i++];
		if(hstr == ' ')
		{
			continue;
		}

		if(i >= SrcStrLen)
			break;
		
		char lstr = SrcStr[i++];
		hexData = ConvertHexChar(hstr);
		lowhexData = ConvertHexChar(lstr);
		
		if((hexData == -1)||(lowhexData == -1))
			break;
		else
			hexData = hexData*16 + lowhexData;

		senddata[hexDataLen++] = (char)hexData;
	}
	senddata.SetSize(hexDataLen);
	return hexDataLen;
}

int CSerialToolDlgFunc::ProcessingData(CStdioFile& file, CString& strData)
{
	CString strLine;
	int phraseCount = 0;				//指令计数，用于判断每包的指令数是否是完整6条
	LONG lActual;

	if (!file.m_pStream)
	{
		return -1;
	}

	lActual = file.GetPosition();
	if(file.ReadString(strLine))
	{
		if(strLine.Left(6).Compare("$GNGGA") == 0)
		{
			strData += strLine + "\r\n";
			++phraseCount;
		}
		else
		{
			file.Seek(lActual, CFile::begin);
		}
	}
	lActual = file.GetPosition();
	if(file.ReadString(strLine))
	{
		if (strLine.Left(6).Compare("$GNGSA") == 0)
		{
			strData += strLine + "\r\n";
			++phraseCount;
		}
		else
		{
			file.Seek(lActual, CFile::begin);
		}
	}
	lActual = file.GetPosition();
	if(file.ReadString(strLine))
	{
		if (strLine.Left(6).Compare("$GNGSA") == 0)
		{
			strData += strLine + "\r\n";
			++phraseCount;
		}
		else
		{
			file.Seek(lActual, CFile::begin);
		}
	}
	lActual = file.GetPosition();
	if(file.ReadString(strLine))
	{
		if (strLine.Left(6).Compare("$GNRMC") == 0)
		{
			strData += strLine + "\r\n";
			++phraseCount;
		}
		else
		{
			file.Seek(lActual, CFile::begin);
		}
	}
	lActual = file.GetPosition();
	if(file.ReadString(strLine))
	{
		if (strLine.Left(6).Compare("$GNVTG") == 0)
		{
			strData += strLine + "\r\n";
			++phraseCount;
		}
		else
		{
			file.Seek(lActual, CFile::begin);
		}
	}
	lActual = file.GetPosition();
	if(file.ReadString(strLine))
	{
		if (strLine.Left(6).Compare("$GNDHV") == 0)
		{
			strData += strLine + "\r\n";
			++phraseCount;
		}
		else
		{
			file.Seek(lActual, CFile::begin);
		}
	}

	if (0 == phraseCount)			//当前语句非法，需要额外处理!!!!!!
	{
		file.ReadString(strLine);	
	}
	
	return phraseCount;
}
static TCHAR* readLine_By0A(TCHAR* dst,const TCHAR* &src)
{
	if (*src == '\0')
	{
		return NULL;
	}
	TCHAR *ret = dst;
	while((*dst++ = *src++) != '\n');
		
	*--dst = '\0';		//'\0'替换'\n'
	return ret;
}
int CSerialToolDlgFunc::ProcessingData(TCHAR* &pFileText, CString& strData)
{
	CString strLine;
	int phraseCount = 0;				//指令计数，用于判断每包的指令数是否是完整6条
	TCHAR arrlActual[200] = {0};
	TCHAR* pFileTextPre;
	if (*pFileText == '\0')
	{
		return -1;
	}

	ZeroMemory(arrlActual, 200);
	pFileTextPre = pFileText;
	readLine_By0A(arrlActual, pFileText);
	strLine = arrlActual;
	if(FALSE == strLine.IsEmpty())
	{
		if(strLine.Left(6).Compare("$GNGGA") == 0)
		{
			strData += strLine + "\r\n";
			++phraseCount;
		}
		else
		{
			pFileText = pFileTextPre;
		}
	}

	ZeroMemory(arrlActual, 200);
	pFileTextPre = pFileText;
	readLine_By0A(arrlActual, pFileText);
	strLine = arrlActual;
	if(FALSE == strLine.IsEmpty())
	{
		if(strLine.Left(6).Compare("$GNGSA") == 0)
		{
			strData += strLine + "\r\n";
			++phraseCount;
		}
		else
		{
			pFileText = pFileTextPre;
		}
	}
		
	ZeroMemory(arrlActual, 200);
	pFileTextPre = pFileText;
	readLine_By0A(arrlActual, pFileText);
	strLine = arrlActual;
	if(FALSE == strLine.IsEmpty())
	{
		if(strLine.Left(6).Compare("$GNGSA") == 0)
		{
			strData += strLine + "\r\n";
			++phraseCount;
		}
		else
		{
			pFileText = pFileTextPre;
		}
	}
	
	ZeroMemory(arrlActual, 200);
	pFileTextPre = pFileText;
	readLine_By0A(arrlActual, pFileText);
	strLine = arrlActual;
	if(FALSE == strLine.IsEmpty())
	{
		if(strLine.Left(6).Compare("$GNRMC") == 0)
		{
			strData += strLine + "\r\n";
			++phraseCount;
		}
		else
		{
			pFileText = pFileTextPre;
		}
	}
		
	ZeroMemory(arrlActual, 200);
	pFileTextPre = pFileText;
	readLine_By0A(arrlActual, pFileText);
	strLine = arrlActual;
	if(FALSE == strLine.IsEmpty())
	{
		if(strLine.Left(6).Compare("$GNVTG") == 0)
		{
			strData += strLine + "\r\n";
			++phraseCount;
		}
		else
		{
			pFileText = pFileTextPre;
		}
	}
	
	ZeroMemory(arrlActual, 200);
	pFileTextPre = pFileText;
	readLine_By0A(arrlActual, pFileText);
	strLine = arrlActual;
	if(FALSE == strLine.IsEmpty())
	{
		if(strLine.Left(6).Compare("$GNDHV") == 0)
		{
			strData += strLine + "\r\n";
			++phraseCount;
		}
		else
		{
			pFileText = pFileTextPre;
		}
	}

	if (0 == phraseCount)			//当前语句非法，需要额外处理，跳过这一行!!!!!!
	{
		ZeroMemory(arrlActual, 200);
		readLine_By0A(arrlActual, pFileText);
	}
	
	return phraseCount;
}

int CSerialToolDlgFunc::RecordData(CStdioFile& file, CString& strData, BOOL bRecTime)
{
	SYSTEMTIME		sysTime;
	CString			strTime, strDataTimed, strTimeErr;

	if (bRecTime)
	{
		GetLocalTime(&sysTime);			//获取当前系统时间

		static WORD		preMilliseconds = sysTime.wMilliseconds;
		int				passedTime = 0;

		strTime.Format("//[%d:%d:%d.%d]\r\n", sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);

		passedTime = sysTime.wMilliseconds - preMilliseconds;

		if (passedTime < 0)
		{
			passedTime = 1000 - preMilliseconds + sysTime.wMilliseconds;
		}

		if ( (passedTime < (RECVDATACYCLE - TIMEDEVIATION)) || (passedTime > (RECVDATACYCLE + TIMEDEVIATION)))
		{
			strTimeErr.Format("//<%d>\r\n", passedTime);
		}
		
		preMilliseconds = sysTime.wMilliseconds;
	}

	strDataTimed = strTimeErr + strTime + strData;
	file.Write(strDataTimed, strDataTimed.GetLength());
	file.Flush();

	return 0;
}