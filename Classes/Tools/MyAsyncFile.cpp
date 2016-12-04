#include "MyAsyncFile.h"

MyAsync::MyAsync()
{
	
}
MyAsync* MyAsync::create()
{
	MyAsync* _myAsync = new MyAsync();
	if (_myAsync&& _myAsync->init())
	{
		return _myAsync;
	}
	else
	{
		CC_SAFE_DELETE(_myAsync);
		return _myAsync;
	}
}
bool MyAsync::init()
{
	m_bIsProcessOver = true;
	return true;
}
void MyAsync::openOnceProcess(const userUseFunc&  _userUseFunc)
{
	m_tUserUseFunc = _userUseFunc;
	m_bIsProcessOver = false;
	std::thread tProcess(&MyAsync::processFunc,this);
	tProcess.detach();
	
}
void MyAsync::processFunc()
{
	m_mutex.lock();
	m_tUserUseFunc();
	m_bIsProcessOver = true;
	m_mutex.unlock();
}
MyAsync::~MyAsync()
{
	
}