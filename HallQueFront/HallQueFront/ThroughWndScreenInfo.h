#pragma once

class CThroughWndScreenInfo
{
public:
	CThroughWndScreenInfo(void);
	~CThroughWndScreenInfo(void);
	CThroughWndScreenInfo(const CThroughWndScreenInfo& obj);
	CThroughWndScreenInfo& operator =(const CThroughWndScreenInfo& obj);

	void SetLocalIP(const CString& ip);
	CString GetLocalIp()const;

	void SetPhyId(int nPhyId);
	int GetPhyId()const;

	void SetPipeId(int nPipeId);
	int GetPipeId()const;

	int GetThroughWndScreenId() const;
	void SetThroughWndScreenId(UINT nWndScreenId);

	int GetWndScreenId()const;
	void SetWndScreenId(int nScreenId);

	int GetComScreenId()const;
	void SetComScreenId(int nComScreenId);

	UINT GetStbID()const;
	void SetStbID(UINT StbID);
private:
	CString m_strLocalIP;//通屏IP
	int m_nPhyId;//通屏物理地址id
	int m_nPipeId;//通屏通道

	int m_nScreenId;//窗口屏id
	int m_nComScreenId;//综合屏id

	UINT m_nThroughWndScreenId;//唯一编号

	UINT m_nStbID;//机顶盒
};
