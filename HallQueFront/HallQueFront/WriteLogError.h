#pragma once

class CWriteLogError
{
public:
	CWriteLogError(void);
	~CWriteLogError(void);
	void WriteErrLog(CString strSockLog);
private:
	CString m_strLogFilePath;
	void WriteLogWithTime(CString strSockLog);
	BOOL AppendWriteFile(CString strText, CString strFileName);
};
