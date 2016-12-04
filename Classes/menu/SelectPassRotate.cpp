#include "SelectPassRotate.h"
#include "game\GameScene.h"
#include "MenuUI.h"
#include "PlayerSelect.h"
#include "LotteryLayer.h"
#include "CoinAndBodyPower.h"
SelectPassRotate* SelectPassRotate::create()
{
	SelectPassRotate* _spr = new SelectPassRotate();
	if (_spr && _spr->init())
	{
		_spr->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(_spr);
	}
	return _spr;
}
Scene* SelectPassRotate::scene()
{
	Scene* _se = Scene::create();
	_se->addChild(SelectPassRotate::create());
	return _se;
}
bool SelectPassRotate::init()
{
	if (!Layer::init())return false;
	auto _dispatcher = Director::getInstance()->getEventDispatcher();
	auto _eventlistener = EventListenerTouchOneByOne::create();
	_eventlistener->onTouchBegan = CC_CALLBACK_2(SelectPassRotate::onTouchbegan , this);
	_eventlistener->onTouchMoved = CC_CALLBACK_2(SelectPassRotate::onTouchMoved, this);
	_eventlistener->onTouchEnded = CC_CALLBACK_2(SelectPassRotate::onTouchEnded, this);
	_dispatcher->addEventListenerWithSceneGraphPriority(_eventlistener , this);
	_eventlistener->setEnabled(true);


	m_bIsTouch = false;
	m_bIsTouchToLeft = false;

	m_fTouchTimeSchedlue = 0.008f;
	m_fTouchTime = 0.000001f;
	m_fScrollLimitTime = 0.35f;
	m_fLimitMinRotate = -(2 * fabs(SELECTPASS_ROTATE_SPACING_ANGLE)* myGameInstance->m_nUnlockChapter - 6.f + (myGameInstance->m_nUnlockChapter-1)*fabs(SELECTPASS_ROTATE_SPACING_ANGLE));
	
	m_fLimitMaxRotate = 0.f;
	m_fScorllStartAngle = 0.f;
	m_fScrollSpeed = 0.f;
	m_nCreateLayerindex = -1;
	m_fNowShowBgOpacity = 0.f;
	m_fWillShowBgOpacity = 0.f;

	m_pSpriteBlur = nullptr;
	m_pblurSprite = nullptr;

	m_nLastStarNum = myGameInstance->m_nStarNum;

	this->initDataOrRes();
	this->createOtherRes();
	//==============创建顶部UI本界面所显示内容优先级最高
	this->createTopUI();


	//================测试代码段
	
	
	//====================
	int _passShowPassIndex = ((myGameInstance->getPassIndex() - 4) <= 0) ? 0 : (myGameInstance->getPassIndex() - 4);
	float _angle = -_passShowPassIndex*SELECTPASS_BTN_SPACING_ANGLE;
	this->mySelectPassRotate(_angle , true);

	string _passNum = "0";
	_passNum[0] = 48 + _passShowPassIndex+1;
	m_strWillShowBgFile = "selectPassRes/chapter" + _passNum + "/bg.jpg";
	_passNum[0] = 48 + _passShowPassIndex;
	m_strShowBgFile = "selectPassRes/chapter0/bg.jpg";

	this->scheduleUpdate();


	if (myGameInstance->getIsSoubdEffectOpen() && myGameInstance->m_nChangeSceneStatus != 1)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/music/menu.ogg" , true);
	}

	if (myGameInstance->m_nChangeSceneStatus == 1)
	{
		m_pClippBlenk = ClippBlenkCustomShape::create();
		this->addChild(m_pClippBlenk);
		m_pClippBlenk->runBlenkAction(Vec2(640, 360), 0.001f, 800.f, 0.5f, [=](){
			m_pClippBlenk->removeFromParentAndCleanup(true);
			m_pClippBlenk = nullptr;
		});

	}
	

	m_pBlenkLayer = LayerColor::create(Color4B(0 , 0 , 0 , 255));
	this->addChild(m_pBlenkLayer);
	m_pBlenkLayer->setVisible(false);
	return true;
}
void SelectPassRotate::initDataOrRes()
{
	LayerColor* _blenkBottom = LayerColor::create(Color4B(255 , 255 , 255 ,255));
	this->addChild(_blenkBottom);

	//m_strFile[21] = 48 + m_nChapterIndex + (_isNext ? 1 : -1);
	int _chapterArticleNum[SELECTPASS_CHAPTER_NUMBER] = { 15, 18  ,23 ,17 };

	m_pSelectPassParentNode = CSLoader::createNode("studioCSB/selectPass.csb");
	this->addChild(m_pSelectPassParentNode);

	m_pCloudNode = myGameInstance->backNodeForCSB("cloudNode", m_pSelectPassParentNode);
	this->createCloudEffect();

	m_pChapterNode = myGameInstance->backNodeForCSB("chapter", m_pSelectPassParentNode);
	/*m_pChapterNode->setPosition(640 , 360);
	m_pChapterNode->setScale(0.1f);*/
	m_tCircleHeart = m_pChapterNode->getPosition();

	m_pGameBgNowShow = (Sprite*)myGameInstance->backNodeForCSB("gameBg0", m_pSelectPassParentNode);
	m_pGameBgWillShow = (Sprite*)myGameInstance->backNodeForCSB("gameBg1", m_pSelectPassParentNode);
	

	int _arrnChapterUnlock[SELECTPASS_CHAPTER_NUMBER] = {30 , 40,50,50};
	for (int i = 0; i < SELECTPASS_CHAPTER_NUMBER; i++)
	{
		m_arrnUnlockChapterNum[i] = _arrnChapterUnlock[i];   

		string _name = "chapter0";
		_name[7] = 48 + i;
		m_vecChapterChildNode.push_back(myGameInstance->backNodeForCSB(_name, m_pChapterNode));

		_name = "bgArticle0";
		_name[9] = 48 + i;
		m_vecChapterBgArticleNode.push_back(myGameInstance->backNodeForCSB(_name, m_pChapterNode));

		for (int j = 0; j < 19; j++)
		{
			Vec2 _onCirclePos = calculateOnCirclePoint(j + i * 19);
			float _angle = calculateOnCircleButtonAngle(j + i * 19);
			ChapterButtonNode* _cbn = ChapterButtonNode::create(_onCirclePos, _angle, i * 19 + j);
			_cbn->setScreenShotFunc(CC_CALLBACK_1(SelectPassRotate::screenShotFunc , this));
			m_vecChapterBtnNodeArray[i].push_back(_cbn);
			m_pChapterNode->addChild(m_vecChapterBtnNodeArray[i][j]);
		}
		for (int k = 0; k < _chapterArticleNum[i]; k++)
		{
			char _articleName[20];
			sprintf(_articleName , "article%d" , k);
			Node* _article = myGameInstance->backNodeForCSB(_articleName, m_vecChapterChildNode[i]);
			JUMPARTICLE* _pJumpArticle = new JUMPARTICLE();
			_pJumpArticle->_article = _article;
			_pJumpArticle->_isOnScreen = true;
			_pJumpArticle->_beginScaleX = _article->getScaleX();
			_pJumpArticle->_beginScaleY = _article->getScaleY();
			m_vecJumpArticle[i].push_back(_pJumpArticle);
		}
		
		//====================解锁章节牌子
		Node* _unlcokButton = myGameInstance->backNodeForCSB("woodGas", m_vecChapterChildNode[i]);
		JUMPARTICLE* _pJumpArticle = new JUMPARTICLE();
		_pJumpArticle->_article = _unlcokButton;
		_pJumpArticle->_isOnScreen = true;
		_pJumpArticle->_beginScaleX = _unlcokButton->getScaleX();
		_pJumpArticle->_beginScaleY = _unlcokButton->getScaleY();
		((Button*)_unlcokButton)->addTouchEventListener(CC_CALLBACK_2(SelectPassRotate::buttonCallBack, this));
		if (i < SELECTPASS_CHAPTER_NUMBER - 1)
		{
			m_vecUnlockChapter.push_back((Button*)_unlcokButton);
		}
		else
		{
			_unlcokButton->setVisible(false);
		}
		
		if (i == myGameInstance->m_nUnlockChapter - 1 || i >= myGameInstance->m_nUnlockChapter)
		{
			//未解锁
			Node* _unlock = myGameInstance->backNodeForCSB("yijiesuo", m_vecChapterChildNode[i]);
			_unlock->setVisible(false);
		}
		else
		{
			//已解锁
			Node* _star = myGameInstance->backNodeForCSB("star", m_vecChapterChildNode[i]);
			_star->setVisible(false);
			((Button*)_unlcokButton)->setEnabled(false);
		}

	}
}
void SelectPassRotate::createCloudEffect()
{
	for (int i = 0; i < myGameInstance->m_nUnlockChapter; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Sprite* _cloud = Sprite::create("menu/cloud.png");
			m_pCloudNode->addChild(_cloud);
			_cloud->setPosition(rand() % (int)(2.72*(i + 1)*STANDARD_WIDTH) + i*STANDARD_WIDTH, rand() % 150 + 500);
			m_vecCloudSprite.push_back(_cloud);
			_cloud->runAction(RepeatForever::create(MoveBy::create(0.1f , Vec2(CCRANDOM_0_1()+ 0.5f , 0))));
			float _scale = CCRANDOM_0_1() + 0.7f;
			if (_scale>1)
			{
				_scale = 1.f;
			}
			_cloud->setScale(_scale);
			_cloud->setFlippedX((rand()%2 == 0)?true:false);
		}
	}
}
void SelectPassRotate::createOtherRes()
{
	m_pLotteryBtn = Button::create("menu/lotteryBtn.png");
	m_pLotteryBtn->setPosition(Vec2(60 , 480) + Vec2(1010 , 180));
	m_pLotteryBtn->addTouchEventListener(CC_CALLBACK_2(SelectPassRotate::buttonCallBack , this));
	this->addChild(m_pLotteryBtn);

	Sprite* _starNumBottom = Sprite::create("menu/starNumBottom.png");
	_starNumBottom->setPosition(100 , 555);
	this->addChild(_starNumBottom);

	Sprite* _starImage = Sprite::create("menu/levelStar.png");
	this->addChild(_starImage);
	_starImage->setPosition(_starNumBottom->getPosition() + Vec2(-60 , 0));

	m_pStarNumLabel = Label::createWithCharMap("number/topUINumber.png",14 , 21, '0');
	m_pStarNumLabel->setPosition(Vec2(60 , 0) + _starNumBottom->getPosition());
	m_pStarNumLabel->setAnchorPoint(Vec2(1.f , .5f));
	this->addChild(m_pStarNumLabel);
	m_pStarNumLabel->setString(__String::createWithFormat("%d" , m_nLastStarNum)->getCString());
}
void SelectPassRotate::update(float _dt)
{
	this->checkShowChapterStatus();
	this->checkJumpArticle();
	this->checkChapterIndex();
	this->checkStarNumber();
}
bool SelectPassRotate::onTouchbegan(Touch* touch, Event* event)
{
	if (m_bIsTouch)return false;
	m_bIsTouch = true;
	m_tTouchStartPos = touch->getLocation();
	m_tFirstTouchPos = touch->getLocation();

	this->dealTouchBeganSetMove(m_tTouchStartPos);
	return true;
}
void SelectPassRotate::onTouchMoved(Touch* touch, Event* event)
{
	if (touch->getLocation().x > m_tTouchStartPos.x)
	{
		m_bIsTouchToLeft = false;
	}
	else
	{
		m_bIsTouchToLeft = true;
	}
	this->dealTouchMoveSetMove(touch->getLocation());
	m_tTouchStartPos = touch->getLocation();
}
void SelectPassRotate::onTouchEnded(Touch* touch, Event* event)
{
	m_bIsTouch = false;
	this->dealTouchEndedSetMove(touch->getLocation());
}
void SelectPassRotate::touchSchedule(float _dt)
{
	m_fTouchTime += m_fTouchTimeSchedlue;
}
void SelectPassRotate::dealTouchBeganSetMove(Vec2 _touchPos)
{
	this->unschedule(CC_SCHEDULE_SELECTOR(SelectPassRotate::touchSchedule));
	this->unschedule(CC_SCHEDULE_SELECTOR(SelectPassRotate::scrollMoveSchedule));
	m_fTouchTime = 0.000001f;
	this->schedule(CC_SCHEDULE_SELECTOR(SelectPassRotate::touchSchedule), m_fTouchTimeSchedlue);
}
void SelectPassRotate::dealTouchMoveSetMove(Vec2 _touchPos)
{
	float _distanceX = _touchPos.x - m_tTouchStartPos.x;
	this->mySelectPassRotate(_distanceX);
}
void SelectPassRotate::dealTouchEndedSetMove(Vec2 _touchPos)
{
	this->unschedule(CC_SCHEDULE_SELECTOR(SelectPassRotate::touchSchedule));
	this->unschedule(CC_SCHEDULE_SELECTOR(SelectPassRotate::scrollMoveSchedule));
	if (m_fTouchTime < m_fScrollLimitTime && fabs(_touchPos.x - m_tFirstTouchPos.x) > 2.5f)
	{
		m_fScorllStartAngle = (_touchPos.x - m_tFirstTouchPos.x) / m_fTouchTime/75;
		if (fabs(m_fScorllStartAngle) > 60.f)
		{
			m_fScorllStartAngle = (m_fScorllStartAngle / fabs(m_fScorllStartAngle))*60.f;
		}
		if (fabs(_touchPos.x - m_tFirstTouchPos.x) < 2.5f)
		{
			m_fScorllStartAngle = (m_fScorllStartAngle / fabs(m_fScorllStartAngle))*15.f;
		}
		m_fScrollSpeed = SELECTPASS_CHAPTER_SCROLL_SPEED*m_fTouchTime*6.f;
		this->schedule(CC_SCHEDULE_SELECTOR(SelectPassRotate::scrollMoveSchedule), 0.01f);
	}
}
void SelectPassRotate::scrollMoveSchedule(float _time)
{
	if (fabs(m_fScorllStartAngle) <= 1.5f)
	{
		m_fScorllStartAngle = 0.f;
		this->unschedule(CC_SCHEDULE_SELECTOR(SelectPassRotate::scrollMoveSchedule));
		return;
	}
	m_fScorllStartAngle = (m_fScorllStartAngle / fabs(m_fScorllStartAngle))*(fabs(m_fScorllStartAngle) - m_fScrollSpeed);
	if (m_pChapterNode->getRotation()>m_fLimitMaxRotate)
	{
		this->unschedule(CC_SCHEDULE_SELECTOR(SelectPassRotate::scrollMoveSchedule));
		return;
	}
	if (m_pChapterNode->getRotation() < m_fLimitMinRotate)
	{
		this->unschedule(CC_SCHEDULE_SELECTOR(SelectPassRotate::scrollMoveSchedule));
		return;
	}
	this->mySelectPassRotate(m_fScorllStartAngle);
}
void SelectPassRotate::mySelectPassRotate(float _slideDistance, bool _passSet)
{ 
	m_pChapterNode->setRotation(m_pChapterNode->getRotation() + (_passSet ? _slideDistance : (_slideDistance / PTM_RATIO / 2.f)));
	
	if (m_pChapterNode->getRotation()>m_fLimitMaxRotate)
	{
		m_pChapterNode->setRotation(m_fLimitMaxRotate);
		this->cloudMoveFunc(0);
	}
	else if (m_pChapterNode->getRotation() < m_fLimitMinRotate)
	{
		m_pChapterNode->setRotation(m_fLimitMinRotate);
		this->cloudMoveFunc(0);
	}
	else
	{
		this->cloudMoveFunc(_slideDistance, _passSet);
	}
}


Vec2 SelectPassRotate::calculateOnCirclePoint(int _index)
{
	float _angle = SELECTPASS_FIRST_BTN_ANGLE + _index * SELECTPASS_BTN_SPACING_ANGLE;
	float _x = (SELECTPASS_ROTATE_RADIUS + 80) * sinf(CC_DEGREES_TO_RADIANS(_angle));
	float _y = (SELECTPASS_ROTATE_RADIUS + 80 )* cosf(CC_DEGREES_TO_RADIANS(_angle));
	return Vec2(_x , _y);
}
float SelectPassRotate::calculateOnCircleButtonAngle(int _index)
{
	float _angle = SELECTPASS_FIRST_BTN_ANGLE + _index * SELECTPASS_BTN_SPACING_ANGLE;
	return _angle; 
}
void SelectPassRotate::checkJumpArticle()
{
	for (int i = 0; i < myGameInstance->m_nUnlockChapter; i++)
	{
		for (auto _jumpArticle: m_vecJumpArticle[i])
		{
			Node* _article = _jumpArticle->_article;
			Vec2 _articlePos = m_vecChapterChildNode[i]->convertToWorldSpace(_article->getPosition());
			Rect _screenRec(-500,0,STANDARD_WIDTH +500, STANDARD_HEIGHT);
			if (_screenRec.containsPoint(_articlePos))   //在屏幕内
			{
				if (_jumpArticle->_isOnScreen)continue;
				this->articleRunActionFunc(_article, true, Vec2(_jumpArticle->_beginScaleX, _jumpArticle->_beginScaleY));
				_jumpArticle->_isOnScreen = true;
			}
			else                                          //在屏幕外
			{
				if (!_jumpArticle->_isOnScreen)continue;
				this->articleRunActionFunc(_article, false, Vec2(_jumpArticle->_beginScaleX, _jumpArticle->_beginScaleY));
				_jumpArticle->_isOnScreen = false;
			}
		}
	}
}
void SelectPassRotate::articleRunActionFunc(Node* _article, bool _isJump, Vec2 _beginScale)
{
	_article->stopAllActions();
	if (_isJump)
	{
		_article->setScaleY(0.01f);
		_article->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=](){
			if (myGameInstance->getIsSoubdEffectOpen())
			{
				SimpleAudioEngine::getInstance()->playEffect("sound/effect/articleJump.ogg");
			}
		}), EaseBackOut::create(ScaleTo::create(0.2f, _beginScale.x, _beginScale.y)), nullptr));
	}
	else
	{
		_article->runAction(Sequence::create(DelayTime::create(0.5f), EaseBackOut::create(ScaleTo::create(0.2f, _beginScale.x, 0.01f)), nullptr));
	}
}
void SelectPassRotate::checkChapterIndex()
{
	/*float _nowRotation = fabs((m_pChapterNode->getRotation()+15.f) / 67.2f);
	m_nChapterIndex = abs((int)((m_pChapterNode->getRotation() + 15.f) / 67.2f));*/
	float _nowRotation = fabs((m_pChapterNode->getRotation()) / 82.2);
	m_nChapterIndex = abs((int)((m_pChapterNode->getRotation()) / 82.2));
	float _distance = _nowRotation - m_nChapterIndex;
	if (_distance > 0.9f && m_nChapterIndex <=SELECTPASS_CHAPTER_NUMBER && m_bIsTouchToLeft)
	{
		this->changeBgAndArticle(1 - _distance , true , true);
	}
	else if (_distance<0.1f && m_nChapterIndex>=1 && !m_bIsTouchToLeft)
	{
		this->changeBgAndArticle(_distance, false , true);
	}
	else
	{
		if (m_nChapterIndex % 2 == 0)
		{
			m_pGameBgNowShow->setOpacity(255);
			m_pGameBgWillShow->setOpacity(0);
		}
		else
		{
			m_pGameBgNowShow->setOpacity(0);
			m_pGameBgWillShow->setOpacity(255);
			
		}
	}
}
void SelectPassRotate::changeBgAndArticle(float _disValuse, bool _isNext, bool _opacity)
{
	if (m_nChapterIndex % 2 == 0)
	{
		string _willShowBgname = m_strWillShowBgFile;
		_willShowBgname[21] = 48 + m_nChapterIndex + (_isNext ? 1 : -1);
		if (m_strWillShowBgFile != _willShowBgname)
		{
			m_pGameBgWillShow->setTexture(_willShowBgname);
			m_strWillShowBgFile = _willShowBgname;
		}
		float _opacity = 255 * _disValuse * 10;
		m_pGameBgNowShow->setOpacity(!_opacity ? (255 - _opacity) : _opacity);
		m_pGameBgWillShow->setOpacity(!_opacity ? _opacity : (255 - _opacity));
	}
	else if (m_nChapterIndex % 2 == 1)
	{
		string _showBgName = m_strShowBgFile;
		_showBgName[21] = 48 + m_nChapterIndex + (_isNext ? 1 : -1);
		if (m_strShowBgFile != _showBgName)
		{
			m_pGameBgNowShow->setTexture(_showBgName);
			m_strShowBgFile = _showBgName;
		}
		float _opacity = 255 * _disValuse * 10;
		m_pGameBgWillShow->setOpacity(!_opacity ? (255 - _opacity) : _opacity);
		m_pGameBgNowShow->setOpacity(!_opacity ? _opacity : (255 - _opacity));
	}
}

void SelectPassRotate::screenShotFunc(int _layerId)
{
	m_nCreateLayerindex = _layerId;
	if (m_nCreateLayerindex == LayerID_ConfirmPassLayer)
	{
		m_pTopUI->setVisible(true);
	}
	else
	{
		m_pTopUI->setVisible(false);
	}

	m_fNowShowBgOpacity = m_pGameBgNowShow->getOpacity();
	m_fWillShowBgOpacity = m_pGameBgWillShow->getOpacity();

	if (m_fNowShowBgOpacity >=m_fWillShowBgOpacity)
	{
		m_pGameBgNowShow->setOpacity(255);
		m_pGameBgWillShow->setOpacity(0);
	}
	else
	{
		m_pGameBgNowShow->setOpacity(0);
		m_pGameBgWillShow->setOpacity(255);
	}

	m_pSpriteBlur = SpriteBlur::create("", BlurModeType::myBlurModeScreenSection);
	m_pSpriteBlur->setBlurRadius(8.f);
	m_pSpriteBlur->setBlurSampleNum(5.f);
	m_pSpriteBlur->setPosition(640 , 360);
	m_pSpriteBlur->setRotation(180);
	m_pSpriteBlur->setFlipX(true);
	this->getParent()->addChild(m_pSpriteBlur);
	this->setVisible(false);
	this->pauseSchedulerAndActions();

	RenderTexture* _textureScreen = RenderTexture::create(m_pSpriteBlur->getContentSize().width, m_pSpriteBlur->getContentSize().height);
	_textureScreen->begin();
	Director::getInstance()->getRunningScene()->visit();
	_textureScreen->end();

	//m_pblurSprite = Sprite::create("menu/blurBg.jpg");
	m_pblurSprite = Sprite::createWithSpriteFrame(_textureScreen->getSprite()->getSpriteFrame());
	this->getParent()->addChild(m_pblurSprite);
	m_pblurSprite->setRotation(180);
	m_pblurSprite->setFlipX(true);
	m_pblurSprite->setPosition(640, 360);

	this->runAction(Sequence::create(DelayTime::create(0.01f), CallFunc::create([this](){
		m_pSpriteBlur->removeFromParentAndCleanup(true);
		m_pSpriteBlur = nullptr;
		switch (m_nCreateLayerindex)
		{
		case LayerID_SelectPlayer:
			this->createSelectPlayerLayer();
			break;
		case LayerID_LotteryLayer:
			this->createLotteryLayer();
			break;
		case LayerID_ConfirmPassLayer:
			this->createConfirmPassLayer();
			break;
		case LayerID_PayBodyLayer:
			this->createCoinAndBodyPowerPay(LayerID_PayBodyLayer);
			break;
		case LayerID_PayCoinLayer:
			this->createCoinAndBodyPowerPay(LayerID_PayCoinLayer);
			break;
		}
		
	}),nullptr));
}
void SelectPassRotate::destroyScreenShot()
{
	m_pGameBgNowShow->setOpacity(m_fNowShowBgOpacity);
	m_pGameBgWillShow->setOpacity(m_fWillShowBgOpacity);

	m_pTopUI->setVisible(true);
	m_pblurSprite->removeFromParentAndCleanup(true);
	m_pblurSprite = nullptr;
	this->setVisible(true);
	this->resumeSchedulerAndActions();
	m_pTopUI->updateHeadImageFindSqlite();
}
void SelectPassRotate::createTopUI()
{
	m_pTopUI = TopUI::create();
	this->addChild(m_pTopUI);
	m_pTopUI->setCustomFunc(CC_CALLBACK_1(SelectPassRotate::screenShotFunc,this));
	m_pTopUI->setLayerID(MyLayerId::LayerID_SelectPass);
	m_pTopUI->m_FuncCreateCoinOrBody = CC_CALLBACK_1(SelectPassRotate::screenShotFunc, this);
}
void SelectPassRotate::createSelectPlayerLayer()
{
	PlayerSelect* _ps = PlayerSelect::create();
	_ps->m_closeBurFunc = CC_CALLBACK_0(SelectPassRotate::destroyScreenShot , this);
	this->getParent()->addChild(_ps);
}
void SelectPassRotate::createLotteryLayer()
{
	LotteryLayer* _lottery = LotteryLayer::create();
	this->getParent()->addChild(_lottery);
	_lottery->m_funcAfterCloseLottery = CC_CALLBACK_0(SelectPassRotate::destroyScreenShot , this);
}
void SelectPassRotate::createConfirmPassLayer()
{
	ConfirmEnterPass* _cep = ConfirmEnterPass::create(myGameInstance->getPassIndex());
	_cep->setCloseScreenShot(CC_CALLBACK_0(SelectPassRotate::destroyScreenShot , this));
	this->getParent()->addChild(_cep);
}
void SelectPassRotate::createCoinAndBodyPowerPay(int _layerID)
{
	if (_layerID == LayerID_PayBodyLayer)
	{
		CoinAndBodyPower* _cabp = CoinAndBodyPower::create(1);
		getParent()->addChild(_cabp);
		_cabp->m_funcCloseScreenShot = CC_CALLBACK_0(SelectPassRotate::destroyScreenShot , this);
	}
	else if (_layerID == LayerID_PayCoinLayer)
	{
		CoinAndBodyPower* _cabp = CoinAndBodyPower::create(0);
		getParent()->addChild(_cabp);
		_cabp->m_funcCloseScreenShot = CC_CALLBACK_0(SelectPassRotate::destroyScreenShot, this);
	}
}
void SelectPassRotate::buttonCallBack(Ref* _sender, Widget::TouchEventType _type)
{
	if (Widget::TouchEventType::BEGAN == _type)
	{
		if (myGameInstance->getIsSoubdEffectOpen())
		{
			SimpleAudioEngine::getInstance()->playEffect("sound/effect/buttonTouch.ogg");
		}
	}
	if (Widget::TouchEventType::ENDED == _type)
	{
		if (_sender == m_pLotteryBtn)
		{
			screenShotFunc(LayerID_LotteryLayer);
			//Application::getInstance()->openURL("http://www.isotoo.com");
		}
		this->dealUnlockChapter(_sender);
	}
}
void SelectPassRotate::checkShowChapterStatus()
{
	for (int i = 0; i < m_vecChapterChildNode.size();i++)
	{
		if (i == m_nChapterIndex - 1 || i == m_nChapterIndex || i == m_nChapterIndex+1)
		{
			for (int j = 0; j < m_vecChapterBtnNodeArray[i].size(); j++)
			{
				m_vecChapterBtnNodeArray[i][j]->setVisible(true);
			}
			m_vecChapterChildNode[i]->setVisible(true);
			
		}
		else
		{
			for (int j = 0; j < m_vecChapterBtnNodeArray[i].size(); j++)
			{
				m_vecChapterBtnNodeArray[i][j]->setVisible(false);
			}
			m_vecChapterChildNode[i]->setVisible(false);
		}
	}
	
}
void SelectPassRotate::checkStarNumber()
{
	if (m_nLastStarNum != myGameInstance->m_nStarNum)
	{
		m_nLastStarNum = myGameInstance->m_nStarNum;
		m_pStarNumLabel->setString(__String::createWithFormat("%d" , m_nLastStarNum)->getCString());
	}
}
void SelectPassRotate::dealUnlockChapter(Ref* _sender)
{
	for (int i = 0; i < m_vecUnlockChapter.size(); i++)
	{
		if (_sender != m_vecUnlockChapter[i])continue;
		if (myGameInstance->m_nStarNum >= m_arrnUnlockChapterNum[i])
		{
			if (myGameInstance->getIsSoubdEffectOpen())
			{
				SimpleAudioEngine::getInstance()->playEffect("sound/effect/unlockChapter.ogg");
			}
			myGameInstance->m_nStarNum -= m_arrnUnlockChapterNum[i];
			mySqliteOpen(SQLITE_FILE_NAME);
			mySqliteSetIntData(SQLITE_TABLE_KEYVALUE, "myStarNum", myGameInstance->m_nStarNum, "", "");
			mySqliteSetIntData(SQLITE_TABLE_KEYVALUE, "unlockChapter", i + 2, "", "");
			mySqliteClose();
			myGameInstance->m_nUnlockChapter = i + 2;
			m_fLimitMinRotate = -(2 * fabs(SELECTPASS_ROTATE_SPACING_ANGLE)* myGameInstance->m_nUnlockChapter - 6.f + (myGameInstance->m_nUnlockChapter - 1)*fabs(SELECTPASS_ROTATE_SPACING_ANGLE));
			m_vecUnlockChapter[i]->setEnabled(false);
			//未解锁
			Node* _unlock = myGameInstance->backNodeForCSB("yijiesuo", m_vecChapterChildNode[i]);
			_unlock->setVisible(true);
			//已解锁
			Node* _star = myGameInstance->backNodeForCSB("star", m_vecChapterChildNode[i]);
			_star->setVisible(false);
		}
		else
		{
			//星星不足
			PromptNotEnoughStar* _pnes = PromptNotEnoughStar::create([=](){});
			this->addChild(_pnes);
		}
	}
}
void SelectPassRotate::cloudMoveFunc(float _rotation, bool _passSet)
{
	if (_passSet)
	{
		float _move = 2.72f*STANDARD_WIDTH*(_rotation) / 82.2f;
		for (int i = 0; i < m_vecCloudSprite.size(); i++)
		{
			m_vecCloudSprite[i]->setPositionX(m_vecCloudSprite[i]->getPositionX() + _move);
		}
	}
	else
	{
		for (int i = 0; i < m_vecCloudSprite.size(); i++)
		{
			m_vecCloudSprite[i]->setPositionX(m_vecCloudSprite[i]->getPositionX() + _rotation/6.f);
			if (fabs(m_vecCloudSprite[i]->getPositionX())>(myGameInstance->m_nUnlockChapter * 2.72f * STANDARD_WIDTH + STANDARD_WIDTH/6))
			{
				int _passShowPassIndex = ((myGameInstance->getPassIndex() - 4) <= 0) ? 0 : (myGameInstance->getPassIndex() - 4);
				float _angle = -_passShowPassIndex*SELECTPASS_BTN_SPACING_ANGLE;
				float _move = 2.72f*STANDARD_WIDTH*_angle / 82.2f / 6.f;
				m_vecCloudSprite[i]->setPositionX(-(m_vecCloudSprite[i]->getContentSize().width + rand() % (int)(myGameInstance->m_nUnlockChapter * 3)) + _move);
			} 
		}
	}
}

void SelectPassRotate::openReplaceBlenk()
{
	m_pBlenkLayer->setVisible(true);
}

SelectPassRotate::~SelectPassRotate()
{
	for (int i = 0; i < myGameInstance->m_nUnlockChapter; i++)
	{
		for (int k = 0; k < m_vecJumpArticle[i].size(); k++)
		{
			CC_SAFE_DELETE(m_vecJumpArticle[i][k]);
		}
	}
}

//============================================================================chapter Button Node
ChapterButtonNode* ChapterButtonNode::create(Vec2 _vec2, float _rotation, int _levelId)
{
	ChapterButtonNode* _cbn = new ChapterButtonNode();
	if (_cbn && _cbn->init(_vec2, _rotation, _levelId))
	{
		_cbn->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(_cbn);
	}
	return _cbn;
}
void ChapterButtonNode::chapterButtonNodeUpdate(Vec2 _circleHeart)
{

}
bool ChapterButtonNode::init(Vec2 _vec2, float _rotation, int _levelId)
{
	if (!Node::init())return false;
	this->setPosition(_vec2);
	this->setRotation(_rotation);
	m_nLevelId = _levelId; 

	this->initDataRes();
	return true;
}
void ChapterButtonNode::initDataRes()
{
	/*Sprite* _levelBottom = Sprite::create("menu/passButtonBottom.png");
	_levelBottom->setPosition(2, -12);
	this->addChild(_levelBottom);*/
	if (myGameInstance->m_vecPassData[m_nLevelId][0] == "unlock")
	{
		m_pButton = Button::create("menu/levelBtn.png");
		this->addChild(m_pButton);
		m_pButton->addTouchEventListener(CC_CALLBACK_2(ChapterButtonNode::buttonCallBack, this));

		int _starNum = atoi(myGameInstance->m_vecPassData[m_nLevelId][1].c_str());
		for (int i = 0; i < 3; i++)
		{
			m_pStarSp[i] = Sprite::create(i<_starNum ? "menu/levelStar.png" : "menu/grayLevelStar.png");
			this->addChild(m_pStarSp[i]);
			m_pStarSp[i]->setPosition(-38 + i * 40, -45);
		}

		this->createLabelNum(m_pPassNum, "number/selectPassNum.png", Vec2::ZERO, Size(32.7f, 40));
		m_pPassNum->setString(__String::createWithFormat("%d", m_nLevelId + 1)->getCString());
	}
	else if (myGameInstance->m_vecPassData[m_nLevelId][0] == "lock")
	{
		Sprite* _lockLevel = Sprite::create("menu/lockPass.png");
		this->addChild(_lockLevel);
		for (int i = 0; i < 3; i++)
		{
			m_pStarSp[i] = Sprite::create("menu/grayLevelStar.png");
			this->addChild(m_pStarSp[i]);
			m_pStarSp[i]->setPosition(-38 + i * 40, -45);
		}
	}
}
void ChapterButtonNode::buttonCallBack(Ref* _sender, Widget::TouchEventType _type)
{
	if (Widget::TouchEventType::BEGAN == _type)
	{
		if (myGameInstance->getIsSoubdEffectOpen())
		{
			SimpleAudioEngine::getInstance()->playEffect("sound/effect/buttonTouch.ogg");
		}
	}
	if (Widget::TouchEventType::ENDED == _type)
	{
		myGameInstance->setPassIndex(m_nLevelId+1);
		m_funcScreenShot(LayerID_ConfirmPassLayer);
	}
}
void ChapterButtonNode::createLabelNum(Label*&_label, string _name, Vec2 _pos, Size _size)
{
	_label = Label::createWithCharMap(_name, _size.width, _size.height, '0');
	_label->setPosition(_pos);
	_label->setString("0");
	this->addChild(_label);
}

//===============================================================================选关二级弹窗
ConfirmEnterPass* ConfirmEnterPass::create(int _level)
{
	ConfirmEnterPass* _cep = new ConfirmEnterPass();
	if (_cep&&_cep->init(_level))
	{
		_cep->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(_cep);
	}
	return _cep;
}
bool ConfirmEnterPass::init(int _level)
{
	if (!Layer::init())return false;
	auto  _dispatcher = Director::getInstance()->getEventDispatcher();
	auto _listener = EventListenerTouchOneByOne::create();
	_listener->setSwallowTouches(true);
	_listener->onTouchBegan = CC_CALLBACK_2(ConfirmEnterPass::onTouchBegan, this);
	_listener->onTouchMoved = CC_CALLBACK_2(ConfirmEnterPass::onTouchMoved, this);
	_listener->onTouchEnded = CC_CALLBACK_2(ConfirmEnterPass::onTouchEnded, this);
	_listener->setEnabled(true);
	_dispatcher->addEventListenerWithSceneGraphPriority(_listener, this);

	m_nLevelIndex = _level;

	this->initDataRes();

	return true;
}

void ConfirmEnterPass::initDataRes()
{
	m_funcCloseScreenShot = nullptr;

	LayerColor* _blenk = LayerColor::create(Color4B(0,0,0 , 100));
	this->addChild(_blenk);

	Node* _pAllDataNode = Node::create();
	this->addChild(_pAllDataNode);
	_pAllDataNode->setPosition(640 , 360);

	Sprite* _bottom = Sprite::create("menu/confirmPassBottom.png");
	_pAllDataNode->addChild(_bottom);

	Sprite* _confirmBottom = Sprite::create("menu/confirmPassBottom.jpg");
	_pAllDataNode->addChild(_confirmBottom);

	Sprite* _titleBottom = Sprite::create("menu/confirmTitle.png");
	_titleBottom->setPosition(0, 170);
	_pAllDataNode->addChild(_titleBottom);

	

	Label* _titleNum = Label::createWithCharMap("number/confirmNumber.png" ,48  ,64, '0');
	_pAllDataNode->addChild(_titleNum);
	_titleNum->setPosition(_titleBottom->getPosition());
	_titleNum->setString(__String::createWithFormat("%d", m_nLevelIndex)->getCString());

	Sprite* _bg = Sprite::create("bg/backGround0.jpg");
	_pAllDataNode->addChild(_bg);
	_bg->setScale(0.28f);

	TMXTiledMap* _tiledMap = TMXTiledMap::create(__String::createWithFormat("map/level%d.tmx" , m_nLevelIndex)->getCString());
	_tiledMap->setPosition(-185, -102);
	_pAllDataNode->addChild(_tiledMap);
	_tiledMap->setScale(0.28f);
	

	m_pConfirmStartBtn = Button::create("menu/payButtonBottom.png", "menu/payButtonBottom0.png", "menu/payButtonBottom.png");
	_pAllDataNode->addChild(m_pConfirmStartBtn);
	m_pConfirmStartBtn->setPosition(Vec2(0 ,-170));
	m_pConfirmStartBtn->addTouchEventListener(CC_CALLBACK_2(ConfirmEnterPass::buttonCallBack , this));

	Sprite* _wirteConfirm = Sprite::create("menu/startWrite.png");
	m_pConfirmStartBtn->addChild(_wirteConfirm);
	_wirteConfirm->setPosition(m_pConfirmStartBtn->getContentSize().width / 2, m_pConfirmStartBtn->getContentSize().height/2 + 7);

	_pAllDataNode->setScale(0.01f);
	_pAllDataNode->runAction(Sequence::create(ScaleTo::create(0.25f, 1.15f), ScaleTo::create(0.15f, 1.0f), nullptr));
}


void ConfirmEnterPass::buttonCallBack(Ref* _sender, Widget::TouchEventType _type)
{
	if (Widget::TouchEventType::BEGAN == _type)
	{
		if (myGameInstance->getIsSoubdEffectOpen())
		{
			SimpleAudioEngine::getInstance()->playEffect("sound/effect/buttonTouch.ogg");
		}
	}
	if (Widget::TouchEventType::ENDED == _type)
	{
		if (myGameInstance->m_nUnlimitBodyPower == 1)
		{
			myGameInstance->m_bIsRestartLevel = false;
			myGameInstance->initRestartChance();
			myGameInstance->setPassIndex(m_nLevelIndex);
			Director::getInstance()->replaceScene(GameScene::scene());
		}
		else
		{
			if (myGameInstance->m_nNowBodyPowerNum >= 3)
			{
				myGameInstance->m_bIsRestartLevel = false;
				myGameInstance->initRestartChance();
				myGameInstance->setPassIndex(m_nLevelIndex);
				Global::getInstance()->getBodyPowerTime()->useBodyPower(3);
				Director::getInstance()->replaceScene(GameScene::scene());
			}
			else  //prompt bodyPower not enough
			{
				BodyPowerLayer* _bodyPowerLayer = BodyPowerLayer::create();
				this->addChild(_bodyPowerLayer);
			}
		}
		
	}
}
void ConfirmEnterPass::onTouchEnded(Touch* touch, Event* event)
{
	this->closeSelfLayer();
}

void ConfirmEnterPass::closeSelfLayer()
{
	if (m_funcCloseScreenShot)
		m_funcCloseScreenShot();
	this->removeFromParentAndCleanup(true);
}
