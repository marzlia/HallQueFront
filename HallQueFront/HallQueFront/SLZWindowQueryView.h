#pragma once
#include "SLZWindow.h"
#include <map>
using namespace std;

class SLZWindowQueryView
{
	friend class CCallThread;
	friend class CInlineQueData;
public:
	SLZWindowQueryView(void);
	~SLZWindowQueryView(void);

	//将保存在文件中的员工信息导入内存
	BOOL LoadWindows();

	//
	//SLZWindow* QueryWindowById(UINT iWindowId);
	//
	BOOL QueryWindowById(UINT iWindowId,SLZWindow& Window);
	//SLZWindow* QueryWindowByCallerId(UINT iCallerId);
	//
	BOOL QueryWindowByCallerId(UINT iCallerId,SLZWindow& Window);
	//SLZWindow* QueryWindowByEvaId(UINT iEvaId);
	BOOL QueryWindowByEvaId(UINT iEvaId,SLZWindow& Window);

	//SLZWindow* QueryWindowByLedPhyId(UINT iLedPhyId);
	BOOL QueryWindowByLedPhyId(UINT iLedPhyId,SLZWindow& Window);

	//SLZWindow* QueryWindowByWndScrId(UINT iWndScrId);
	BOOL QueryWindowByWndScrId(UINT iWndScrId,SLZWindow& Window);

	//SLZWindow* QueryWindowByWndComId(UINT iWndComId);
	BOOL QueryWindowByWndComId(UINT iWndComId,SLZWindow& Window);
	
	int GetAllThroughID(int throughID[]);
	void Clear();
//	BOOL InitLoginMap();//初始化登录表
private:
	//std::map<UINT,SLZWindow*> m_mapCallerIdWindow;
	std::map<UINT,SLZWindow> m_mapCallerIdWindow;
	//std::map<UINT,SLZWindow*> m_mapEvaIdWindow;
	std::map<UINT,SLZWindow> m_mapEvaIdWindow;
	//std::map<UINT,SLZWindow*> m_mapLedPhyIdWindow;
	std::map<UINT,SLZWindow> m_mapLedPhyIdWindow;
	//std::map<UINT,SLZWindow*> m_mapWndScrIdWindow;
	std::map<UINT,SLZWindow> m_mapWndScrIdWindow;
	//std::map<UINT,SLZWindow*> m_mapWndComIdWindow;
	std::map<UINT,SLZWindow> m_mapWndComIdWindow;
public:
	//std::map<UINT,SLZWindow*> m_mapIdWindow;
	std::map<UINT,SLZWindow> m_mapIdWindow;
};
