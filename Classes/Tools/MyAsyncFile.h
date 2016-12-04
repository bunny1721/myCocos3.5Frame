#pragma once
#include "Tools\MacroFile.h"

class MyAsync
{
private:
	typedef std::function<void()> userUseFunc;
public:
	MyAsync();
	~MyAsync();
	bool getIsProcessOver(){ return m_bIsProcessOver; };

	static MyAsync* create();
	bool init();
	void openOnceProcess(const userUseFunc&  _userUseFunc);
private:
	void processFunc();
protected:
	bool m_bIsProcessOver;
	std::mutex m_mutex;
	userUseFunc m_tUserUseFunc;
}; 