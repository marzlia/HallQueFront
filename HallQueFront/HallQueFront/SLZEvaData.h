#pragma once
#include "DataDef.h"
class SLZEvaData
{
public:
	SLZEvaData(void) : m_iEvaluatorId(0){}
	~SLZEvaData(void){}

	//获取评价器地址
	int GetEvaluatorId() const
	{
		return m_iEvaluatorId;
	}
	//设置评价器地址
	void SetEvaluatorId(int iEvaluatorId)
	{
		m_iEvaluatorId = iEvaluatorId;
	}
	//获取评价等级
	EvaLevel GetEvaLevel() const
	{
		return m_evaLevel;
	}
	//设置评价等级
	void SetEvaLevel(EvaLevel evaLevel)
	{
		m_evaLevel = evaLevel;
	}

private:
	int m_iEvaluatorId;		//评价器地址
	EvaLevel m_evaLevel;	//评价等级
};
