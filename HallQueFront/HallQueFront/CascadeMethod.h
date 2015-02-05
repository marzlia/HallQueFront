#pragma once
#include "CommonConvert.h"
#include "QueueInfo.h"
#include "SLZStaff.h"
#include "SLZWindow.h"
#include "ComputeFuncationTime.h"
#include "SwingCard.h"
#include "CardLevel.h"

class CCascadeMethod
{
public:
	CCascadeMethod(void);
	~CCascadeMethod(void);

	void DeleteQueInfo(CString strQueId);
	void DeleteStaffInfo(CString strStaffId);
	void DeleteStbInfo(CString strStbID);
private:
	CComputeFuncationTime m_ComputerTime;
	CCommonConvert convert;
	CString m_QueInfoPath;
	CString m_StaffInfoPath;
	CString m_WindowInfoPath;
	CString m_SwingInfoPath;
	CString m_connect_path;
	CString m_cardLev_path;
	CARDCONNECTINFO m_cardConnectInfo;
	CMap<int,int,CQueueInfo,CQueueInfo&> m_map_QueInfo;
	CMap<int,int,SLZStaff,SLZStaff&> m_map_StaffInfo;
	CMap<int,int,SLZWindow,SLZWindow&> m_map_WindowInfo;
	CMap<int,int,CSwingCard,CSwingCard&> m_map_SwingInfo;
	CMap<int,int,CCardLevel,CCardLevel&> m_map_CardLevelInfo;
 	BOOL ReadQueInfoFromFile();
	BOOL ReadStaffInfoFromFile();
	BOOL ReadWindowInfoFromFile();
	BOOL ReadConfigCardInfoFromFile();
	BOOL WriteConfigCardInfoToFile();
	BOOL WriteQueInfoToFile();
	BOOL WriteStaffInfoToFile();
	BOOL WriteWindowInfoToFile();
	BOOL WriteConnectInfoToFile();
	BOOL ReadConnectInfoFromFile();
	BOOL WriteCardLevelToFile();
	BOOL ReadCardLevelFromFile();
	BOOL ReadStbInfoFromFile();
	BOOL WriteStbInfoToFile();
};
