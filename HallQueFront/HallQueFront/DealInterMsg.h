#pragma once
#include <string>
using namespace std;

class SLZData;

typedef enum SlzDataType{
	enumSlzDataSerialid,
	enumSlzDatabussName,
	enumSlzDataqueId,
	enumSlzDataqueManNum,
	enumSlzDataqueNum,
	enumSlzDataintQueNum,
	enumSlzDatacardType,
	enumSlzDatacardNum,
	enumSlzDatacustName,
	enumSlzDatacustLevel,
	enumSlzDatatakeNumTime,
	enumSlzDatastaffId,
	enumSlzDataphoneNum,
	enumSlzDatasendMsg
};

class CDealInterMsg
{
public:
	CDealInterMsg(void);
	~CDealInterMsg(void);
public:
	static void ProduceSendInterMsg(const CString& queManNum,string& retSendMsg);//������������ȡ�ŵı���
	static void ProduceRetInterMsg(const SLZData* pData,
		UINT inlineNum/*���е�ǰ����*/,string& retMsg/*���ص�msg*/);//��������ȡ�ţ����صı���
	static void ProduceSendInNumMsg(const CString& queManNum,string& retInlineNumMsg);//��������ȡ�ŷ����Ŷ���������
	static void ProduceRetInNumMsg(UINT inlineNum,string& retInlineMsg);//��������ȡ�ŷ����Ŷ���������
	static void ProduceSendCallMsg(const CStringArray& queManNumArray,string& retCallMsg,const CString& organId,BOOL bIsUserPower);//����ʱ����ȡ�Ų�������Ϣ
	static void ProduceRetCallMsg(BOOL isSucced,string& retMsg,const SLZData* pData);//��������ȡ����Ϣ����
	static void ProduceRetAlertCallerMsg(const CString& queManNum,string& retAlertMsg);//����ȡ��ʱ���ÿͻ�������������
	static void ProduceBrodcastRetInNumMsg(const CString& queManNum,UINT inlineNum,string& retBrodcastNumMsg);//�㲥�Ŷ�����
public:
	static BOOL AnaRetInterMsg(const string& retMsg,SLZData* pData,UINT* pInLineNum=NULL);//�������ص�����ȡ�ű��ģ��õ���ǰ���е����ź��Ŷ�����
	static BOOL AnaRetInNumMsg(const string& retMsg,UINT* pInlineNum);//�������صĵ�ǰ�Ŷ������ı���,�õ���ǰ�����Ŷ�����

	static BOOL AnaSendInterMsg(const string& retSendMsg,CString& queManNum,CString& organId,CString& organName);//�������͵�����ȡ�ű��ģ��õ�ȡ�ŵĶ���ID
	static BOOL AnaSendInNumMsg(const string& retSendMsg,CString& queManNum);//�������͵Ļ�ȡ�ŶӶ��������ı���,�õ�����ID


	static BOOL AnaSendCallMsg(const string& retSendMsg,CStringArray& queManNumArray,CString& organId,BOOL* pIsUsePower);//�������͵ĺ�������ȡ�Ų�������Ϣ

	static BOOL AnaRetCallMsg(const string& retMsg,BOOL* pIsSucced,SLZData* pData);//�������ص�����ȡ�ź��в�������Ϣ/����

	static BOOL AnaRetAlterCallerMsg(const string& retAlterMsg,CString& queManNum);//�������صĺ�����������Ϣ

	static BOOL AnaRetBrodcastNumMsg(CString& queManNum,UINT* pInlineNum,string& retBrodcastNumMsg);//�����㲥��������
private:
	static BOOL GetMsgData(const string& msg,SlzDataType dataType,string& data);
	static void StringToTime(const string& strTime,CTime& time);
};
