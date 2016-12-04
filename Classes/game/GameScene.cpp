#include "GameScene.h"
#include "GameLayer.h"
#include "GameUI.h"
#include "GameBG.h"
Scene* GameScene::scene()
{
	Scene* se = Scene::create();
	GameScene* gs = GameScene::create();
	se->addChild(gs);
	return se;
}
GameScene* GameScene::create()
{
	GameScene* gs = new GameScene();
	if (gs && gs->init())
	{
		gs->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(gs);
	}
	return gs;
}


bool GameScene::init()
{
	if (!Layer::init())return false;
	Global::getInstance()->m_pGameScene = this;

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	m_pListener = EventListenerTouchAllAtOnce::create();

	m_pListener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
	m_pListener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
	m_pListener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(m_pListener, this);

	m_pListener->setEnabled(true);
	m_bCanToch = true;
	
	myGameInstance->m_nLevelEatStarNum = 0;
	//-----------------添加背景层
	m_pGameBgLayer = GameBgLayer::create();
	this->addChild(m_pGameBgLayer, Priority_GameBG);
	
	//----------------添加游戏层
	m_pGameLayer = GameLayer::create();
	this->addChild(m_pGameLayer, Priority_GameLayer);
	
	//----------------添加UI层
	m_pGameUILayer = GameUILayer::create(MySceneType::SCENE_GAME);
	this->addChild(m_pGameUILayer, Priority_GameUI);
	

	//---------------------------------测试代码段
	
	
	////======================模糊

	//
	//SpriteBlur* _spriteBlur = SpriteBlur::create("obstacle/star0.png");
	//_spriteBlur->setPosition(700 , 295);
	//this->addChild(_spriteBlur , 9999);
	//_spriteBlur->setBlurRadius(15.f);
	//_spriteBlur->setBlurSampleNum(6.f);

	//_spriteBlur->setBlendFunc({GL_DST_COLOR , GL_SRC_COLOR});
	//_spriteBlur->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });
	//_spriteBlur->setScale(1.8f);

	//Sprite* _sp = Sprite::create("obstacle/star0.png");
	//this->addChild(_sp, 9999);
	//_sp->setPosition(700, 295);
	
	//--------------------测试代码段

	//----------------开启更新
	this->scheduleUpdate();


	//if (myGameInstance->m_nStarNum == 1)

	myGameInstance->m_nStarNum == 1;
	myGameInstance->m_nChangeSceneStatus = 3;
	return true;
}


void GameScene::update(float _dt)
{
	//---------背景更新
	m_pGameBgLayer->updateGameBG();
	//--------------游戏更新
	m_pGameLayer->updateGameLayer(_dt);
	//-----------------ui更新
	m_pGameUILayer->updateGameUI();
}


void GameScene::onTouchesBegan(const std::vector<Touch*>& _touch, Event* event)
{
	for (int i = 0; i < _touch.size();i++)
	{
		m_pGameBgLayer->touchBegan(_touch[i], event, _touch[i]->getID());
		m_pGameLayer->touchBegan(_touch[i], event, _touch[i]->getID());
		m_pGameUILayer->touchBegan(_touch[i], event, _touch[i]->getID());
	}

	//m_bCanToch = false;
}

void GameScene::onTouchesMoved(const std::vector<Touch*>& _touch, Event* event)
{
	for (int i = 0; i < _touch.size(); i++)
	{
		m_pGameBgLayer->touchMoved(_touch[i], event, _touch[i]->getID());
		m_pGameLayer->touchMoved(_touch[i], event, _touch[i]->getID());
		m_pGameUILayer->touchMoved(_touch[i], event, _touch[i]->getID());
	}
	
}

void GameScene::onTouchesEnded(const std::vector<Touch*>& _touch, Event* event)
{
	for (int i = 0; i < _touch.size(); i++)
	{
		
		m_pGameBgLayer->TouchEnded(_touch[i], event, _touch[i]->getID());
		m_pGameLayer->TouchEnded(_touch[i], event, _touch[i]->getID());
		m_pGameUILayer->TouchEnded(_touch[i], event, _touch[i]->getID());
	}

	//m_bCanToch = true;
}



//-------------------------暂停游戏场景
void GameScene::pauseGameScene()
{
	Global::getInstance()->pauseOrResumeNode(this , true);
	this->pause();
}
//-------------------------恢复暂停的游戏场景
void GameScene::resumeGameScene()
{
	Global::getInstance()->pauseOrResumeNode(this, false);
	this->resume();
}
//------------------------------游戏时间缩放
void GameScene::gameTimeScale(float _scale)
{
	Director::getInstance()->getScheduler()->setTimeScale(_scale);
}


GameScene::~GameScene()
{
	
}

