/************************************************************************/
/*     Author:	Songyz                                                  */
/*     Time:	2013.11.14                                              */
/************************************************************************/

#pragma once

class CommonStrMethod
{
public:
	CommonStrMethod(void);
	~CommonStrMethod(void);
public:

	/**
	desc:		宽字节字符串 转 单字节字符串

	dstBuf:		目标字符串缓冲区
	src:		源字符串

	return:		成功，返回 TRUE;否则，返回 FALSE
	*/
	static BOOL WChar2Char(char* dstBuf, size_t len, const wchar_t* src);

	/**
	desc:		单字节字符串 转 宽字节字符串

	dstBuf:		目标字符串缓冲区
	src:		源字符串

	return:		成功，返回 TRUE;否则，返回 FALSE
	*/
	static BOOL Char2WChar(wchar_t* dstBuf,size_t len,const char *src);

	/**
	desc:		在字符串中查找子字符串第一次出现的下标
	strSrc:		源字符串
	strFind:	要查找的字符串

	return:		查找成功，返回所查找字符串第一次出现的下标；
	否则，返回 -1
	*/
	static int StrFind(CString strSrc, CString strFind);

	/**
	desc:		在字符串中查找子字符串第一次出现的下标, 不区分大小写
	strSrc:		源字符串
	strFind:	要查找的字符串

	return:		查找成功，返回所查找字符串第一次出现的下标；
	否则，返回 -1
	*/
	static int StrFindNoCase(CString strSrc, CString strFind);

	/**
	desc:		将源字符串中的字符串替换成新字符串

	strSrc:		源字符串
	strFrom:	要替换的字符串
	strTo:		用来替换的新字符串

	return:		返回替换后的新字符串
	*/
	static CString StrReplace(CString strSrc, CString strFrom, CString strTo);

	/**
	desc:			将字符串根据分隔符分割成字符串数组, 
					分隔符可以是一个字符，也可以是多个字符。
					例如，_T("\n") 或 _T(" ") 或 _T(" \n,.。")

	strSrc:			源字符串
	arrStrDest:		字符串数组，保存分割好的字符串
	strDivisionSet:	分隔符集

	return:			成功，返回 TRUE；否则，返回 FALSE
	*/
	static BOOL StrSplit(CString strSrc, CStringArray& arrStrDest, CString strDivisionSet);

	
	static int FindWcharNoCase(CString& strSrc, TCHAR chFind);

	/**
	desc:		比较两个宽字符是否相等, 不区分大小写

	return:		相等，返回 TRUE；否则，返回 FALSE
	*/
	static BOOL CompareWcharNoCase(WCHAR ch1, WCHAR ch2);

	/**
	desc:				在字符串中获取属性值

	strSrc:				源字符串
	strProfileName:		要获取的属性名
	strProfileValue:	保存获取到的属性值

	return:				成功，返回 TRUE；否则，返回 FALSE
	*/
	static BOOL StrGetProfileStr(CString strSrc, CString strProfileName, 
		CString& strProfileValue, WCHAR wchSplitNameValue, WCHAR wchSplitProfile);

	/**
	desc:				在字符串中获取属性值，不区分属性名大小写

	strSrc:				源字符串
	strProfileName:		要获取的属性名
	strProfileValue:	保存获取到的属性值

	return:				成功，返回 TRUE；否则，返回 FALSE
	*/
	static BOOL StrGetProfileStrNoCase(CString strSrc, CString strProfileName, 
		CString& strProfileValue, WCHAR wchSplitNameValue, WCHAR wchSplitProfile);

	/**
	desc:		整型数字字符串 转成 整型数字

	strNum:		数字字符串

	return:		返回 转换好的整型数字
	*/
	static int Str2Int(CString strNum);

	/**
	desc:		整型数字 转成 整型数字字符串 

	iNum:		整型数字

	return:		返回 转换好的整型数字字符串
	*/
	static CString Int2Str(int iNum);


	//////////////////////////////////////////////////////////////

	/**
	desc:		获取当前程序所在路径,最后包含'\'

	return:		返回 当前程序所在路径
	*/
	static CString GetModuleDir(void);

	static CString GetModuleName(void);

	/**
	desc:			获取某文件夹下的文件全路径名

	strPath:		文件夹路径
	strarrFileList: 保存文件列表的字符串数组

	return:			返回 当前程序所在路径
	*/
	static int GetAbsNameListOfDir(CString strPath, CStringArray& strarrFileList);

	/**
	desc:			获取某文件夹下的文件全路径名

	strPath:		文件夹路径
	strarrFileList: 保存文件列表的字符串数组

	return:			返回 当前程序所在路径
	*/
	static int GetNameListOfDir(CString strPath, CStringArray& strarrFileList);

	static BOOL GetLocalHostNameAndIP(CString& strHostName, CString& strHostIP);

	static CString GetLocalHostName();

	static CString GetLocalIP();

	static BOOL StrIsNumbers(CString strNum);

	static CString StrClearUnvisibleChar(CString& strSrc);

	static BOOL PathFileExist(CString strPath);

	static BOOL CreatePath(CString strPath);

	static BOOL CreateFile(CString strFile);

	static CString GetPathFromAbsName(CString strAbsName);

	static CString GetNameFromAbsName(CString strAbsName);

	static BOOL IsAbsPath(CString strAbsPath);

	static CString GetPrivateProfileString(CString strAppName, CString strKeyName, 
		CString strDefault, CString strFileName);

	static BOOL WritePrivateProfileString(CString strAppName, CString strKeyName, 
		CString strProfile, CString strFileName);

	static USHORT NetToHostShort(USHORT uNetShort);

	static CString NetToHostIP(IN_ADDR& in_addrNet);


///////////////////////////////		private method		/////////////////////////////////
private:
	static BOOL _CreatePath(CString strPath);
};
