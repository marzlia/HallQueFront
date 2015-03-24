#ifndef  PRINTDEF_H
#define PRINTDEF_H

typedef enum _tagEnumPrintObjectEnum
{
	enumPrintType = 0,//业务类型(队列)
	enumPrintWaitNum = 1,//排队人数
	enumPrintClientNum = 2,//客户号码
	enumPrintClientName = 3,//客户姓名
	enumPrintClientLevel  = 4, //客户星级
	enumPrintCustProduct = 5 //客户持有产品
}EnumPrintObject;

typedef enum _tagEnumPrintStatus
{
	enumPrintErr = -1,//打印错误
	enumPrintPaperOut = -2,//缺纸
	enumPrintOffline = -3,//掉线
	enumPrintPrinting = 0,//正在打印，可能打印机出现故障
	enumPrintNormal = 1,//打印机正常
	enumPrintNoDefPrinter = -4,//没有默认打印机
	enumPrintFailedJob = -5//获取job失败
}EnumPrintStaus;

const CString CS_printType = _T("所在队列");
const CString CS_printWaitNum = _T("等候人数");
const CString CS_printClientNum = _T("客户号码");
const CString CS_printClientName = _T("客户姓名");
const CString CS_printClientLevel = _T("客户星级");
const CString CS_printCustProduct = _T("客户持有产品");

const double PI = 3.14159265359;
#endif