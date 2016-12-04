#include "GameBG.h"
#include "GameLayer.h"
GameBgLayer* GameBgLayer::create()
{
	GameBgLayer* gbl = new GameBgLayer();
	if (gbl && gbl->init())
	{
		gbl->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(gbl);
	}
	return gbl;
}

bool GameBgLayer::init()
{
	if (!Layer::init())return false;
	Global::getInstance()->m_pGameBg = this;
	m_bIsPauseBg = false;
	
	//------初始化
	this->initDataOrRes();

	return true;
}
void GameBgLayer::updateGameBG()
{
	if (m_bIsPauseBg)return;
}
void GameBgLayer::touchBegan(Touch* pTouch, Event* event ,int _toucId)
{
	
}
void GameBgLayer::touchMoved(Touch* pTouch, Event* event, int _toucId)
{

}
void GameBgLayer::TouchEnded(Touch* pTouch, Event* event, int _toucId)
{
	
}

//---------------------------------初始化资源或数据
void GameBgLayer::initDataOrRes()
{
	/*Sprite* _bg = Sprite::create(__String::createWithFormat("bg/backGround%d.jpg", _chapterIndex)->getCString());
	_bg->setPosition(STANDARD_WIDTH/2.f ,STANDARD_HEIGHT/2.f);
	this->addChild(_bg);*/
	//_bg->setVisible(false);
}
 