#ifndef _HALLQUEFRONT_DATADEF_H
#define _HALLQUEFRONT_DATADEF_H


#define EVA_MSG WM_USER+1019
#define CALL_MSG WM_USER+1117

#define MYBUFLEN 256
#define DATABUFLEN 2048

#define INTERPORT 10036
/*ָ�����ö��*/
typedef enum _tagCmdType
{
	cmdNotDefined		= 0,		//δ����
	cmdLogin			= 1,		//��¼
	cmdQuit				= 2,		//�˳�
	cmdCall				= 3,		//����
	cmdRecall			= 4,		//�غ�
	cmdDiscard			= 5,		//����
	cmdWait				= 6,		//������һλ׼��
	cmdEvaReq			= 7,		//��������
	cmdPause			= 8,		//��ͣ
	cmdResume			= 9,		//�ָ�
	cmdCallNum			= 10,		//�����ض�����
	cmdCallSec          = 11,		//���б���
	cmdCallMana			= 12,		//���д��þ���
	cmdCallBusc			= 13,		//����ҵ�����
	cmdExChange			= 14,		//ת�ƶ���
	//��������ʾ�Լ���������
	callerCmdShowAdd	= 15,		//��ʾ��ַ
	callerCmdShowSuc	= 16,		//��ʾ�ɹ�(������ɹ�)
	callerCmdShowFail	= 17,		//��ʾʧ��
	callerCmdShowNum	= 18,		//��ʾʣ������
	callerCmdShowAlarm	= 19		//�����ε���
} CmdType;

//typedef enum _tagCtlCmdType
//{
//	//��������ʾ�Լ���������
//	callerCmdNotDefine	= 0,		//δ����
//	callerCmdShowAdd	= 1,		//��ʾ��ַ
//	callerCmdShowSuc	= 2,		//��ʾ�ɹ�(������ɹ�)
//	callerCmdShowFail	= 3,		//��ʾʧ��
//	callerCmdShowNum	= 4,		//��ʾʣ������
//	callerCmdShowAlarm	= 5			//�����ε���
//} CtlCmdType;

/*���۵ȼ�ö��*/
typedef enum _tagEvaLevel
{
	evaNone			= -1,		//δ����
	evaBad				= 0,		//������
	evaNormal		= 1,		//һ��
	evaGood			= 2,		//����
	evaPefect			= 3		//�ǳ�����
} EvaLevel;

/*������*/
typedef enum _tagCardTypeEnum
{
	cardNotDefine		= 0,		//δ����
	cardIDCard			= 1,		//���֤
	cardMagCard			= 2			//���п�
} CardType;



/*
ˢ������Ϣ
*/
typedef struct _tagCardInfo
{
public:
	_tagCardInfo(){
		nAttchPageID = -1;
		iCustLevel = 0;
	}
	~_tagCardInfo(){}
public:
	CardType iCardType;//������
	CString strCardNumber;//����,���֤��
	UINT iCustLevel;//�û�����,���ڶԽ�Զ�̷�����(�д�����)
	CString strCustName;//�û�����������ˢ���֤
	CString strAttchQueID;//�����Ǹ����еĶ���ID
	int nAttchPageID;//�����ĸ�����
}CARDINFO,*PCARDINFO;

typedef struct _tagLogicVariables//ϵͳ�ۺ��߼�����
{
	BOOL IsAutoCloseComputer;//�Զ��ػ�
	CTime CloseComputerTime;//�ػ�ʱ��
	BOOL IsAutoClearData;//ϵͳ�Զ��������
	CTime ClearDataTime;//�Զ��������ʱ��
	BOOL IsLackPaperAlarm;//ȱֽ����
	BOOL ExchangeQue;//ת�ƶ���
	BOOL ExchangeWindow;//ת�ƴ���
	BOOL IsAutoSendToServer;//�Ƿ����������ݵ������
	BOOL IsUseJtts;			//�Ƿ�ʹ��jtts������
	WCHAR strOrganNmae[MYBUFLEN];//��������
	WCHAR strOrganID[MYBUFLEN];//��������//֧�д���
	WCHAR strParOrgName[MYBUFLEN];//������������
	WCHAR strParOrgID[MYBUFLEN];//��������ID
	int playSpeed;			//JTTS �����ٶ�
	UINT iNumberCallType;	//���ֲ��Ÿ�ʽ 
	BOOL IsAutoChangePage;	//��ťѡ���Ժ��Զ�����������
	BOOL IsOpenJudgeShortMsg;//�Ƿ��������������ֲ���ʱ�����Ͷ��ŵ��ֻ�
	BOOL IsOpenInterNum;//�Ƿ�������ȡ��
	WCHAR strInterIP[MYBUFLEN];//����ȡ�ŷ�����ip
}LogicVariables,*pLogicVariables;
/*
��ʾȱֽ������ʾ�Ⱥ�
*/
typedef struct _tagShowVariables
{
	BOOL bShowNoPage;
	BOOL bShowWait;
	BOOL bSendMsg;
}ShowVariables,*pShowVariables;

typedef struct _tagShowInfomation
{
	CString strAdMsg;
	CString strShowMsg;
	UINT iShowTime;
}ShowInfo,*pShowInfo;

///д��������
typedef struct _tagWriteComMsg
{
public:
	_tagWriteComMsg(){
		memset(buf,0,DATABUFLEN);
		length = 0;
	}
	int length;
	char buf[DATABUFLEN];
}WriteComMsg;

#define STRING_QUEUEID_CALL			(_T("[�ŶӺ���]"))			//�����ŶӺ���
#define STRING_CUSTNAME_CALL		(_T("[�ͻ�����]"))			//���пͻ�����
#define STRING_WINDOWNAME			(_T("[��������]"))			//��������

#define COLORCODE_RED				(_T("#0#"))			//��ɫ
#define COLORCODE_GREEN				(_T("#1#"))			//��ɫ
#define COLORCODE_YELLOW			(_T("#2#"))			//��ɫ

//typedef enum _tagGenderEnum
//{
//	gdNotDefined		= 0,	//δ����
//	gdMale				= 1,	//��
//	gdFemale			= 2		//Ů
//} Gender;

#define MYGENDER_NONE	 2014		//δ����
#define MYGENDER_MALE		 2015		//��
#define MYGENDER_FEMALE	 2016		//Ů

typedef USHORT GENDER;
	
#define COLORREF_RED				0x0000FF			//��ɫ
#define COLORREF_GREEN				0x00FF00			//��ɫ
#define COLORREF_YELLOW				0x00FFFF			//��ɫ

#endif //_HALLQUEFRONT_DATADEF_H