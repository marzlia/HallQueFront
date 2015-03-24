#ifndef FUZHOU_H
#define FUZHOU_H
#include <string>
using namespace std;
typedef struct recvCustLev
{
	BOOL isSucced;//是否成功
	int highestCardLev;//最高卡级别
	int cardLev;//刷卡级别
	int custLev;//分行客户级别
	int custLevAll;//总行级别
	int creditCardLev;//信用卡最高级别
	string strAubm;//金融资产总额
	string strAubmMavg;//金融资产总额月日均
	string strAubmQavg;//金融资产季日均
	string strAubmYavg;//金融资产年日均
	int proForCust[15];//客户持有产品,依次为:理财、基金、电子国债、凭证国债、三方、保险、个贷、信用卡、短信通知、稳盈灵通账户、个人网银、移动银行、ATM跨行转账、银联无卡支付、信用卡关联还款、代发工资
}CustLev;

#endif