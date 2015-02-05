#pragma once
#include <list>
#include "SLZEvaData.h"
#include "afxmt.h"
using namespace std;
class CEvaCmdData
{
public:
	CEvaCmdData(void);
	~CEvaCmdData(void);
	void Add(const SLZEvaData& data);
	BOOL GetHeadEvaData(SLZEvaData& data);
	int GetEvaListCount();
private:
	list<SLZEvaData> m_list_evaData;
	CMutex m_mtEvaData;
};
