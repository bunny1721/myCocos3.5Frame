#include "GameUI.h"
#include "GameLayer.h"
#include "GameBG.h"
#include "GameScene.h"
#include "menu\MenuUI.h"
#include "menu\TeachLayer.h"
GameUILayer* GameUILayer::create(int _type)
{
	GameUILayer* gul = new GameUILayer();
	if (gul && gul->init(_type))
	{
		gul->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(gul);
	}
	return gul;
}


bool GameUILayer::init(int _type)
{
	if (!Layer::init())return false;
	m_nUIType = _type;
	if (m_nUIType == MySceneType::SCENE_SELECTPASS)
	{
		this->initSelectPassUI();
	}
	else if (m_nUIType == MySceneType::SCENE_GAME)
	{
		Global::getInstance()->m_pGameUI = this;

		this->initButtonAndOther();
	}

	
	
	return true;
}
void GameUILayer::initSelectPassUI()
{

}
void GameUILayer::initButtonAndOther()
{
}
void GameUILayer::initPlayerBlood()
{
	
}
void GameUILayer::initProgressTimer()
{
	

}
void GameUILayer::initLabelNumber()
{

}
void GameUILayer::buttonCallBack(Ref* _sender, Widget::TouchEventType _type)
{
	if (_type == Widget::TouchEventType::BEGAN)
	{
	}
	else if (_type == Widget::TouchEventType::ENDED)
	{
	}
	else if (_type == Widget::TouchEventType::MOVED)
	{
	}
}
void GameUILayer::pauseGameAndBG()
{
	//Global::getInstance()->m_pGameBg->pauseBGLayer();
	Global::getInstance()->m_pGameLayer->pauseGameLayer();
}
void GameUILayer::resumeGameAndBG()
{
	Global::getInstance()->m_pGameLayer->resumeGameLayer();
}

void GameUILayer::updateGameUI()
{
	this->checkNumber();
}


void GameUILayer::touchBegan(Touch* pTouch, Event* event, int _toucId)
{
}
void GameUILayer::touchMoved(Touch* pTouch, Event* event, int _toucId)
{
}
void GameUILayer::TouchEnded(Touch* pTouch, Event* event, int _toucId)
{
	
}
void GameUILayer::pauseUILayer()
{
	Global::getInstance()->pauseOrResumeNode(this, true);
	this->pause();
}
void GameUILayer::resumeUILayer()
{
	Global::getInstance()->pauseOrResumeNode(this, false);
	this->resume();
}

void GameUILayer::createButton(Button* &_Btn, string _pathName, Vec2 _pos)
{
	_Btn = Button::create(_pathName);
	_Btn->setPosition(_pos);
	_Btn->addTouchEventListener(CC_CALLBACK_2(GameUILayer::buttonCallBack, this));
	this->addChild(_Btn);
}
void GameUILayer::createBtnColdBar(ProgressTimer* &_bar, string _name, Vec2 _pos)
{
	_bar = ProgressTimer::create(Sprite::create(_name));
	_bar->setPosition(Vec2(_pos.x, _pos.y + 5));
	_bar->setType(ProgressTimerType::RADIAL);
	_bar->setPercentage(0);
	_bar->setReverseDirection(true);
	this->addChild(_bar);
}
void GameUILayer::createNormalSprite(Sprite*& _sp, string _name, Vec2 _pos)
{
	_sp = Sprite::create(_name);
	_sp->setPosition(_pos);
	this->addChild(_sp);
}
void GameUILayer::createLabelNum(Label*&_label, string _name, Vec2 _pos, Size _size)
{
	_label = Label::createWithCharMap(_name, _size.width, _size.height, '0');
	_label->setPosition(_pos);
	_label->setString("0");
	this->addChild(_label);
}
void GameUILayer::onFrameEvent(Bone* _bone, const string _event, int originFrameIndex, int currentFrameIndex)
{

}
void GameUILayer::checkNumber()
{
}
