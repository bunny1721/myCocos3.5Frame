#include "MyTimeManager.h"
#include "CommonFile.h"
BodyPowerTimeCheck* BodyPowerTimeCheck::create()
{
	BodyPowerTimeCheck* bptc = new BodyPowerTimeCheck();
	if (bptc&&bptc->init())
	{
		return bptc;
	}
	else
	{
		CC_SAFE_DELETE(bptc);
	}
	return bptc;
}
bool BodyPowerTimeCheck::init()
{
	//----------恢复时间暂定为60
	m_nRegainTime = 60;
	m_bIsStopProcess = true;
	return true;
}
void BodyPowerTimeCheck::useBodyPower(int _decreaseNodyCount)
{
	mySqliteOpen(SQLITE_FILE_NAME);
	if (Global::getInstance()->m_nNowBodyPowerNum == Global::getInstance()->m_nBodyLimit)
	{
		mySqliteSetIntData( SQLITE_TABLE_KEYVALUE,"LastTimeForBody", time(nullptr) ,  "" , "");
	}
	Global::getInstance()->m_nNowBodyPowerNum -= _decreaseNodyCount;
	if (Global::getInstance()->m_nNowBodyPowerNum <= 0)
		Global::getInstance()->m_nNowBodyPowerNum = 0;
	mySqliteSetIntData(SQLITE_TABLE_KEYVALUE , "bodyPowerNum", Global::getInstance()->m_nNowBodyPowerNum , "" , "");
	mySqliteClose();
	this->readyProcessForTime();
}
void BodyPowerTimeCheck::readyProcessForTime()
{ 
	mySqliteOpen(SQLITE_FILE_NAME);
	Global::getInstance()->m_nNowBodyPowerNum = mySqliteGetIntData(SQLITE_TABLE_KEYVALUE,"value" , "key" ,"bodyPowerNum");
	if (Global::getInstance()->m_nNowBodyPowerNum >= Global::getInstance()->m_nBodyLimit)return;
	long int _nowTime = time(nullptr); 
	long int _lastTime = mySqliteGetIntData(SQLITE_TABLE_KEYVALUE, "value", "key", "LastTimeForBody");
	m_nTimeDistance = difftime(_nowTime, _lastTime);
	m_nSurPlusNum = m_nRegainTime - m_nTimeDistance % m_nRegainTime;
	if (m_nTimeDistance >= m_nRegainTime)
	{
		int _addBody = m_nTimeDistance / m_nRegainTime;
		Global::getInstance()->m_nNowBodyPowerNum += _addBody;
		if (Global::getInstance()->m_nNowBodyPowerNum >= Global::getInstance()->m_nBodyLimit)
		{
			//----体力已满
			Global::getInstance()->m_nNowBodyPowerNum = Global::getInstance()->m_nBodyLimit;
		}
		else
		{
			this->openProcessForTime();
		}
		mySqliteSetIntData(SQLITE_TABLE_KEYVALUE,"bodyPowerNum", Global::getInstance()->m_nNowBodyPowerNum , "" ,"");
	}
	else
	{
		this->openProcessForTime();
	}
	mySqliteClose();
}
void BodyPowerTimeCheck::openProcessForTime()
{
	thread timeProcess(&BodyPowerTimeCheck::calcuLateFunc, this);
	timeProcess.detach();
}
void BodyPowerTimeCheck::calcuLateFunc()
{
	m_mTimeMute.lock();
	m_bIsStopProcess = false;
	while (true)
	{
		if (m_bIsStopProcess)
		{
			break;
		}
		m_nSurPlusNum--;
		if (m_nSurPlusNum <= 0)
		{
			Global::getInstance()->m_nNowBodyPowerNum += 1;
			if (Global::getInstance()->m_nNowBodyPowerNum > Global::getInstance()->m_nBodyLimit)
			{
				//体力已满
				Global::getInstance()->m_nNowBodyPowerNum = Global::getInstance()->m_nBodyLimit;
				mySqliteOpen(SQLITE_FILE_NAME);
				mySqliteSetIntData(SQLITE_TABLE_KEYVALUE, "bodyPowerNum", Global::getInstance()->m_nNowBodyPowerNum, "", "");
				mySqliteClose();
				m_bIsStopProcess = true;
			}
			else
			{
				//体力加一
				m_nSurPlusNum = m_nRegainTime;
				mySqliteOpen(SQLITE_FILE_NAME);
				mySqliteSetIntData(SQLITE_TABLE_KEYVALUE,"LastTimeForBody", time(nullptr) , "" , "");
				mySqliteSetIntData(SQLITE_TABLE_KEYVALUE, "bodyPowerNum", Global::getInstance()->m_nNowBodyPowerNum, "", "");
				mySqliteClose();
			}
		}
		this->translateTime(m_nSurPlusNum);

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		sleep(1);
#else
		Sleep(1000);
#endif
	}
	m_bIsStopProcess = false;
	m_mTimeMute.unlock();
} 
void BodyPowerTimeCheck::translateTime(int _time)
{
	int _timeMin = _time / 60;
	int _timeSec = _time % 60;
}

////------------------------------------------------------------------------------每日登陆的时间管理
DayLoginTimeCheck* DayLoginTimeCheck::create()
{
	DayLoginTimeCheck* dltc = new DayLoginTimeCheck();
	if (dltc && dltc->init())
	{
		return dltc;
	}
	else
	{
		CC_SAFE_DELETE(dltc);
	}
	return dltc;
}
bool DayLoginTimeCheck::init()
{
	
	return true;
}

int DayLoginTimeCheck::checkLoginDayNum()
{
	mySqliteOpen(SQLITE_FILE_NAME);
	long _continueDay = mySqliteGetIntData(SQLITE_TABLE_KEYVALUE, "value", "key", "continueLoginDayNum");
	long _lastDay = mySqliteGetIntData(SQLITE_TABLE_KEYVALUE, "value", "key", "lastLoginDay");
	mySqliteClose();
	if (isNotSameDay())
	{
		_continueDay++;
		if (_continueDay >= 7)
		{
			_continueDay = 1;
		}
		mySqliteSetIntData(SQLITE_TABLE_KEYVALUE , "continueLoginDayNum", _continueDay , "" ,"");
	}
	return _continueDay;
}
bool DayLoginTimeCheck::isNotSameDay()
{
	mySqliteOpen(SQLITE_FILE_NAME);
	long int _lastDay = mySqliteGetIntData(SQLITE_TABLE_KEYVALUE, "value", "key", "lastLoginDay");
	mySqliteClose();
	if (getNowDayNum() > _lastDay) 
	{
		return true;
	}
	else
	{
		return false;
	}
}
int DayLoginTimeCheck::getNowDayNum()
{
	return time(nullptr)/86400;
}


//--------------------------------------- 限时体力无限
NoLimitBodyPowerTime* NoLimitBodyPowerTime::create()
{
	NoLimitBodyPowerTime* _npt = new NoLimitBodyPowerTime();
	if (_npt && _npt->init())
	{
		
	}
	else
	{
		CC_SAFE_DELETE(_npt);
	}
	return _npt;
}

bool NoLimitBodyPowerTime::init()
{
	m_nProcessStatus = 0;
	return true;
}

void NoLimitBodyPowerTime::checkNoLimitTime()
{
	if (myGameInstance->m_nUnlimitBodyPower != 0)
	{
		mySqliteOpen(SQLITE_FILE_NAME);
		m_nLastTime = mySqliteGetIntData(SQLITE_TABLE_KEYVALUE, "key", "value", "lastLimitTime");
		m_nTargetTime = mySqliteGetIntData(SQLITE_TABLE_KEYVALUE, "key", "value", "unlimitTargetTime");
		mySqliteClose();
		if (m_nLastTime > m_nTargetTime)
		{
			//--------------------ulimit bodypower is over
			myGameInstance->m_nUnlimitBodyPower = 0;
			mySqliteOpen(SQLITE_FILE_NAME);
			mySqliteSetIntData(SQLITE_TABLE_KEYVALUE, "isUnlimitBodyPower", 0, "" , "");
			mySqliteClose();
		}
		else
		{
			//-----------------------continue unlimited body
			thread _tProcess(&NoLimitBodyPowerTime::processTimeCalculate, this);
			_tProcess.detach();
			m_nProcessStatus = 1;
		}
	}
}

void NoLimitBodyPowerTime::processTimeCalculate()
{
	m_mutex.lock();

	while (true)
	{
		if (m_nProcessStatus == 2)
		{	
			this->dealStartNewLimitTime();
		}
		m_nLastTime = time(nullptr);
		if (m_nLastTime > m_nTargetTime)
		{
			//--------------------ulimit bodypower is over
			myGameInstance->m_nUnlimitBodyPower = 0;
			mySqliteOpen(SQLITE_FILE_NAME);
			mySqliteSetIntData(SQLITE_TABLE_KEYVALUE, "isUnlimitBodyPower", 0, "", "");
			mySqliteClose();
			break;
		}
		else
		{
			mySqliteOpen(SQLITE_FILE_NAME);
			mySqliteSetIntData(SQLITE_TABLE_KEYVALUE, "lastLimitTime", m_nLastTime, "", "");
			mySqliteClose();
		}

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		sleep(1);
#else
		Sleep(1000);
#endif
	}
	m_nProcessStatus = 0;
	m_mutex.unlock();
}
void NoLimitBodyPowerTime::dealStartNewLimitTime()
{
	m_nLastTime = time(nullptr);
	m_nTargetTime = m_nLastTime + LIMIT_TATOL_TIME;
	myGameInstance->m_nUnlimitBodyPower = 1;
	mySqliteOpen(SQLITE_FILE_NAME);
	mySqliteSetIntData(SQLITE_TABLE_KEYVALUE, "unlimitTargetTime", m_nTargetTime, "", "");
	mySqliteSetIntData(SQLITE_TABLE_KEYVALUE, "isUnlimitBodyPower", 1, "", "");
	mySqliteClose();
}

void NoLimitBodyPowerTime::startNewLimitTime()
{
	if (m_nProcessStatus == 0)
	{
		this->dealStartNewLimitTime();
		thread _tProcess(&NoLimitBodyPowerTime::processTimeCalculate, this);
		_tProcess.detach();
		m_nProcessStatus = 1;
	}
	else if (m_nProcessStatus == 1)
	{
		m_nProcessStatus = 2;
	}
}
