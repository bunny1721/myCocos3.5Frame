#include "GameLayer.h"
#include "GameScene.h"
#include "GameBG.h"
#include "GameUI.h"
GameLayer* GameLayer::create()
{
	GameLayer* gl = new GameLayer();
	if (gl&&gl->init())
	{
		gl->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(gl);
	}

	return gl;
}
bool GameLayer::init()
{
	if (!Layer::init())return false;
	LayerColor* _layer = LayerColor::create(Color4B(255,255,255,0));
	this->addChild(_layer);

	Global::getInstance()->m_pGameLayer = this;

	this->initDataOrRes();
	

	return true;
}
void GameLayer::initDataOrRes()
{
	
}
void GameLayer::updateGameLayer(float _dt)
{
	
}


void GameLayer::touchBegan(Touch* pTouch, Event* event, int _toucId)
{
	
}
void GameLayer::touchMoved(Touch* pTouch, Event* event, int _toucId)
{
	
}


void GameLayer::TouchEnded(Touch* pTouch, Event* event, int _toucId)
{
	
} 

void GameLayer::pauseGameLayer()
{
	Global::getInstance()->pauseOrResumeNode(this ,true);
	this->pause();
}
void GameLayer::resumeGameLayer()
{
	Global::getInstance()->pauseOrResumeNode(this, false);
	this->resume();
}
void GameLayer::checkImpact()
{
	
}
void GameLayer::buttonCallBack(Ref* _sender, Widget::TouchEventType _type)
{
	if (_type == Widget::TouchEventType::ENDED)
	{

	}
}
void GameLayer::onFrameEvent(Bone* _bone, const string _event, int originFrameIndex, int currentFrameIndex)
{ 

}
GameLayer::~GameLayer()
{
	
}

