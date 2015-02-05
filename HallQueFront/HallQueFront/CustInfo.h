
class CCustInfo
{
public:
	CCustInfo(void);
	virtual ~CCustInfo(void);
	void SetName(CString name){m_Name=name;}
	void SetGender(CString gender){m_Gender=gender;}
	//void SetFolk(CString folk){m_Folk=folk;}
	void SetIdNum(CString idnum){m_IdNum=idnum;}
	//void SetAddress(CString address){m_Address=address;}
	//void SetDepart(CString depart){m_Depart=depart;}
	//void SetUsefulLife(CString usefullife){m_UsefulLife=usefullife;}
	void SetBirth(CString birth){m_Birth=birth;}
	//void SetPHMsg(unsigned char phmsg[]){memcpy(PHMsg,phmsg,sizeof(PHMsg));}
public:
	CString m_Name;
	CString m_Gender;
	//CString m_Folk;
	CString m_IdNum;
	//CString m_Address;
	//CString m_Depart;
	//CString m_UsefulLife;
	CString m_Birth;
	//unsigned char PHMsg[1024];
};
