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
private:
	CString m_strLocalIP;//ͨ��IP
	int m_nPhyId;//ͨ�������ַid
	int m_nPipeId;//ͨ��ͨ��

	int m_nScreenId;//������id
	int m_nComScreenId;//�ۺ���id

	UINT m_nThroughWndScreenId;//Ψһ���
};
