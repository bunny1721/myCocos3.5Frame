#include "LoadingMenu.h"
#include "MainMenu.h"
#define ALL_RES_COUNT 363
LoadingLayer* LoadingLayer::create()
{
	LoadingLayer* _ll = new LoadingLayer();
	if (_ll && _ll->init())
	{
		_ll->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(_ll);
	}
	return _ll;
}
Scene* LoadingLayer::scene()
{
	Scene* _se = Scene::create();
	_se->addChild(LoadingLayer::create());
	return _se;
}
bool LoadingLayer::init()
{
	if (!Layer::init())return false;
	m_nLoadingNumber = 0;
	m_nPointShowCount = 0;
	m_nJumpCount = 0;
	m_bIsReplace = false;
	for (int i = 0; i < 5; i++)
	{
		m_bIsJumpPlayer[i] = false;
	}

	this->initData();

	return true;
}
void LoadingLayer::initData()
{
	
}

void LoadingLayer::imageLoading()
{
	
}
void LoadingLayer::imageCallFunc()
{
	this->addResCount();
}
void LoadingLayer::musicLoading()
{
	
}
void LoadingLayer::musicCallFunc()
{
	this->addResCount();
}

void LoadingLayer::initGlobalData()
{
	m_thread = thread(&LoadingLayer::threadGlobalInit , this);
	m_thread.detach();
}
void LoadingLayer::threadGlobalInit()
{
	Global::getInstance();
	this->addResCount();
}
void LoadingLayer::addResCount()
{
	m_nLoadingNumber++;
	if (m_bIsReplace)return;
	if (m_nLoadingNumber >= ALL_RES_COUNT)
	{
		m_bIsReplace = true;
		TransitionFade* _tFade = TransitionFade::create(0.5f, MainMenu::scene() , Color3B(0 ,0 , 0));
		Director::getInstance()->replaceScene(_tFade);
	}
}