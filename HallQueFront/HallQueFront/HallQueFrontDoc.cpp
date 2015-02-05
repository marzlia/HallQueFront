// HallQueFrontDoc.cpp : CHallQueFrontDoc 类的实现
//

#include "stdafx.h"
#include "HallQueFront.h"

#include "HallQueFrontDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHallQueFrontDoc

IMPLEMENT_DYNCREATE(CHallQueFrontDoc, CDocument)

BEGIN_MESSAGE_MAP(CHallQueFrontDoc, CDocument)
END_MESSAGE_MAP()


// CHallQueFrontDoc 构造/析构

CHallQueFrontDoc::CHallQueFrontDoc()
{
	// TODO: 在此添加一次性构造代码

}

CHallQueFrontDoc::~CHallQueFrontDoc()
{
}

BOOL CHallQueFrontDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CHallQueFrontDoc 序列化

void CHallQueFrontDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CHallQueFrontDoc 诊断

#ifdef _DEBUG
void CHallQueFrontDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHallQueFrontDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CHallQueFrontDoc 命令
