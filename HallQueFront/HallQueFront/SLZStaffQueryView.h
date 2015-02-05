#pragma once
#include "SLZStaff.h"
#include <map>
using namespace std;

class SLZStaffQueryView
{
public:
	SLZStaffQueryView(void);
	~SLZStaffQueryView(void);
	//将保存在文件中的员工信息导入内存
	BOOL LoadStaffs();
	//
	SLZStaff* QueryStaffById(CString strStaffId);
	//
	void Clear();
public:
//	CMap<CString, LPCTSTR, SLZStaff*, SLZStaff*&> m_mapIdStaff;
	std::map<CString,SLZStaff*> m_mapIdStaff;
};
