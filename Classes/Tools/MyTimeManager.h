#pragma once
#include "MacroFile.h"
//------------------����ʱ��Ĺ���
class BodyPowerTimeCheck
{
public:
	static BodyPowerTimeCheck* create();
	void useBodyPower(int _decreaseNodyCount);
	void readyProcessForTime();
private:
	bool init();
	void bodyTimeCalculate();
	void openProcessForTime();
	void calcuLateFunc();
	void translateTime(int _time);
protected:
	int m_nRegainTime;
	int m_nTimeDistance;
	int m_nSurPlusNum;

	bool m_bIsStopProcess;
	mutex m_mTimeMute;
};
//---------------------ÿ�յ�½��ʱ�����
class DayLoginTimeCheck
{
public:
	static DayLoginTimeCheck* create();
	bool init();
	int checkLoginDayNum();
	bool isNotSameDay();
private:
	int getNowDayNum();
	
protected:

};

#define LIMIT_TATOL_TIME 10800
//--------------------------------------- ��ʱ��������
class NoLimitBodyPowerTime
{
public:
	static NoLimitBodyPowerTime* create();
	
	void startNewLimitTime();
private:
	bool init();
	void checkNoLimitTime();
	void processTimeCalculate();
	void dealStartNewLimitTime();
protected:
	mutex m_mutex;
	long int m_nLastTime;
	long int m_nTargetTime;
	int m_nProcessStatus;
	
};