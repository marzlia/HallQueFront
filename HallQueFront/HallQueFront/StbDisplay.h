#pragma once

class StbDisplay
{
public:
	StbDisplay();
	~StbDisplay();
	//更新标题
	BOOL StbUpdateTitleHtml(CString strTitle, CString strStbId);
	//更新通知公告
	BOOL StbUpdateNoticeHtml(CString strNotice, CString strStbId);
	//更新呼叫信息
	BOOL StbUpdateCallMsg(CString strIMsg, CString strStbId);

	BOOL Start();
private:

	//导入液晶显示的配置参数
	void LoadDisplayConf();
	//液晶显示初始化
	BOOL InitDisplay();
	//启动多媒体信息发布服务端
	BOOL InitNetServer();

	//将颜色标识转换成网页的颜色标识
	CString CovertColorString(CString strSrc);

	BOOL WriteToStbHtmlFile(CString& strHtmlStr, CString strStbId, CString strHtmlFileName);

	BOOL WriteToFile(CString& strText, CString& strFilePath);

private:

	CString m_strModuleDir;//应用程序目录
	CString m_strWebRootDir;//网页文件根目录

	CString m_strHtmlTitleBegin;//标题的网页文件开始部分
	CString m_strHtmlTitleEnd;//标题的标题网页文件后面部分
	CString m_strHtmlNoticeBegin;//广告通知的网页文件开始部分
	CString m_strHtmlNoticeEnd;//广告通知的标题网页文件后面部分
	CString m_strHtmlInstMsgBegin;//紧急通知的网页文件开始部分
	CString m_strHtmlInstMsgEnd;//紧急通知的网页文件后面部分
	CString m_strHtmlQueueInfoBegin;//列表的网页文件开始部分
	CString m_strHtmlQueueInfoEnd;//列表的网页文件后面部分

	PROCESS_INFORMATION m_piNetServer;//多媒体信息发布服务端进程信息
};

