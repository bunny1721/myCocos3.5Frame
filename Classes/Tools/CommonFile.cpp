#include "CommonFile.h"
#include "JavaAndCpp.h"

//================================================全局方法的使用
Global::Global(){
	m_pMyAsync = MyAsync::create();
	m_pMySqlite = MySqilteObjec::create();
	m_pBodyPowerTime = BodyPowerTimeCheck::create();
	m_pDayLoginCheck = DayLoginTimeCheck::create();
	m_pMyJniClass = new MyJniClass();
	m_pNoLimitBodyPowerTime = NoLimitBodyPowerTime::create();

	m_nBodyLimit = 30;
	m_bIsOneStartOneControl = false;
	m_bIsRestartLevel = false;
	m_nStarNum = 0;
	m_nPlayBGMusicStatus = 0;
	m_nChangeSceneStatus = 0;

	m_bIsMusicOpen = true;
	m_bIsSoundEffectOpne = true;

	//---------------------每次游戏开始读取一些数据
	this->readSomeDataEveryOnce();
	

	m_nRandomCodeValue = rand() % 1000 + 1000;
	m_nLeverRestartChance = 0;
	

	srand(time(0));
};
Global::~Global(){
	CC_SAFE_DELETE(m_pMyAsync);
	CC_SAFE_DELETE(m_pMySqlite);
	CC_SAFE_DELETE(m_pBodyPowerTime);
	CC_SAFE_DELETE(m_pDayLoginCheck);
	CC_SAFE_DELETE(m_pMyJniClass);
	CC_SAFE_DELETE(m_pNoLimitBodyPowerTime);
};
Global* Global::getInstance()
{
	if (!s_globalInstace)
	{
		s_globalInstace = new Global();
	}
	return s_globalInstace;
}

string Global::G2U(const std::string& src)
{
	//	std::wstring wstr(src.begin(), src.end());
	//
	//	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	//	string a = conv.to_bytes(wstr);
	
	return "";
}
//----------------------------------------------------从csb得到的节点中获取子节点
Node* Global::backNodeForCSB(const string _name, Node* _parent)
{
	if (!_parent)
	{
		return nullptr;
	}
	if (!_name.compare(_parent->getName()))
	{
		return _parent;
	}
	for (auto &_child : _parent->getChildren())
	{
		if (_child)
		{
			Node* __use = Global::getInstance()->backNodeForCSB(_name, _child);
			if (__use)
			{
				return __use;
			}
		}
	}
	return nullptr;
}
//--------------------------------------------------------暂停或者恢复节点
void Global::pauseOrResumeNode(Node* _node, bool _pause)
{
	if (!_node)
	{
		return;
	}
	for (auto &_child : _node->getChildren())
	{
		if (_child)
		{
			if (_pause)
				_child->pause();
			else
				_child->resume();
			Global::getInstance()->pauseOrResumeNode(_child, _pause);
		}
	}

}
//---------------------------------------------------------------------设置某个节点的绑定一个schedule来单独设置运动
void Global::setActionBind(Node*& _node, Scheduler*& _newSchedule, ActionManager*& _newActionManager)
{
	Scheduler* defaultScheduler = Director::getInstance()->getScheduler();
	_newSchedule = new Scheduler();
	defaultScheduler->scheduleUpdate(_newSchedule, 0, false);
	_newActionManager = new ActionManager();
	_newSchedule->scheduleUpdate(_newActionManager, 0, false);
	_node->setActionManager(_newActionManager);
}
//-------------------------------------------------------------------创建精灵动画 单图的
void Global::createSpriteAnimation(Sprite*& _sp, string _name, int _count, int _loopCount , float _time)
{
	int numPos = -1;
	for (string::iterator _strInfo = _name.begin(); _strInfo != _name.end(); _strInfo++)
	{
		if (*_strInfo == '.')
			break;
		else
			numPos++;
	}
	Animation* ani = Animation::create();
	for (int i = 0; i < _count; i++)
	{
		_name.at(numPos) = 48 + i;
		ani->addSpriteFrameWithFile(_name);
	}
	ani->setDelayPerUnit(_time);
	ani->setLoops(_loopCount);
	_sp->runAction(Animate::create(ani));
}
void Global::createActionForFrameSprite(ActionInterval* &_act, string _name, int _count, int _loopCount)
{
	int imgId = _name.find('.');
	Animation* ani = Animation::create();
	for (int i = 0; i < _count; i++)
	{
		_name.at(imgId - 1) = 48 + i;
		ani->addSpriteFrameWithFile(_name);
	}
	ani->setLoops(_loopCount);
	ani->setDelayPerUnit(1.f / 15);
	_act = Animate::create(ani);
}
void Global::spriteCleanUpBlackBottom(Sprite*& _sprite)
{
	if (!_sprite)return;
	BlendFunc _blen = { GL_SRC_ALPHA, GL_ONE };
	_sprite->setBlendFunc(_blen);
}
void Global::parabolaCalculateParameter(Vec2 _posA, Vec2 _posB, Vec2 _posC, float& _NumA, float& _NumB, float& _NumC)
{
	_NumB = ((_posC.y - _posB.y) / (_posC.x*_posC.x - _posB.x*_posB.x) - (_posB.y - _posA.y) / (_posB.x*_posB.x - _posA.x*_posA.x)) / ((_posC.x - _posB.x) / (_posC.x*_posC.x - _posB.x*_posB.x) - (_posB.x - _posA.x) / (_posB.x*_posB.x - _posA.x*_posA.x));
	_NumA = ((_posA.y - _posB.y) - _NumB*(_posA.x - _posB.x)) / (_posA.x*_posA.x - _posB.x*_posB.x);
	_NumC = _posA.y - _NumA*_posA.x*_posA.x - _NumB*_posA.x;
}
float Global::parabolaCalculateY(float _a, float _b, float _c, float _x)
{
	return _a*(_x*_x) + _b*_x + _c;
}
int Global::dealSoundEffect(int _index, int _status, bool _isLoop, int _stopID)
{
	
	return 0;
}
void Global::dealSoundMusic(int _index, int _status, bool _isLoop)
{
	
}
void Global::shakeNode(Node* _node, Vec2 _beforeStartPos, float _extent, int _count, float _time)
{
	if (_node->getActionManager()->getNumberOfRunningActionsInTarget(_node) != 0)
		_node->stopAllActions();
	_node->runAction(Repeat::create(Sequence::create(
		MoveBy::create(_time, Vec2(0, _extent)), MoveBy::create(_time, Vec2(-_extent, 0)),
		MoveBy::create(_time, Vec2(0, -_extent)), MoveBy::create(_time, Vec2(_extent, 0)),
		CallFunc::create([=](){
		_node->setPosition(_beforeStartPos);
	}), nullptr), _count));
}
template<typename T, typename... Args>
void Global::myPrint(int nCount, T _head, Args... _info)
{
	/*va_list arglist;
	va_start(arglist, nCount);
	for (int i = 0; i < nCount; i++)
	{
		T _data = va_arg(arglist, T);
		log(_data);
	}
	va_end(arglist);*/
	
}
void Global::readSomeDataEveryOnce()
{
	m_pMySqlite->openMySqlite(SQLITE_FILE_NAME);
	m_nNowBodyPowerNum = m_pMySqlite->searchIntDataFromTable(SQLITE_TABLE_KEYVALUE, "value", "key", "bodyPowerNum");;
	m_nCoin = m_pMySqlite->searchIntDataFromTable(SQLITE_TABLE_KEYVALUE, "value", "key", "coin");
	m_nPlayerLeveRestartLimit = m_pMySqlite->searchIntDataFromTable(SQLITE_TABLE_KEYVALUE, "value", "key", "limitRestartCount");

	m_nUnlockChapter = m_pMySqlite->searchIntDataFromTable(SQLITE_TABLE_KEYVALUE, "value", "key", "unlockChapter");

	m_nStarNum = m_pMySqlite->searchIntDataFromTable(SQLITE_TABLE_KEYVALUE, "value", "key", "myStarNum");

	m_nUnlimitBodyPower = m_pMySqlite->searchIntDataFromTable(SQLITE_TABLE_KEYVALUE, "value", "key", "isUnlimitBodyPower");
	int _lockPassIndex = -1;
	m_nLastUnlcokPass = SELECTPASS_CHAPTER_NUMBER * 19;
	for (int i = 0; i < SELECTPASS_CHAPTER_NUMBER; i++)
	{
		for (int j = 0; j < 19;j++)
		{
			m_vecPassData[j + i * 19].push_back(m_pMySqlite->searchStringDataFromTable(SQLITE_TABLE_PASSDATA , "status" ,"passNum" , __String::createWithFormat("%d" , j+i*19)->getCString()));
			m_vecPassData[j + i * 19].push_back(m_pMySqlite->searchStringDataFromTable(SQLITE_TABLE_PASSDATA, "starNum", "passNum", __String::createWithFormat("%d", j + i * 19)->getCString()));
			
			if (m_vecPassData[j + i * 19][0] == "lock" && _lockPassIndex == -1)
			{
				_lockPassIndex = j + i * 19;
				m_nLastUnlcokPass = j + i * 19;
			}
		}
	}
	if (_lockPassIndex != -1)
		m_nPassIndex = _lockPassIndex;
	else
		m_nPassIndex = 0;
	m_pMySqlite->closeMySqlite();



	

	
}

//----------------------------------------------------------------------------实现光照效果裁切 目前仅限圆形

ClippLight* ClippLight::create(string _stencil, string _light, float _time, float _angle, bool _isToUp)
{
	ClippLight* cl = new ClippLight();
	if (cl && cl->init(_stencil, _light, _time, _angle, _isToUp))
	{
		cl->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(cl);
	}
	return cl;
}

bool ClippLight::init(string _stencil, string _light, float _time, float _angle, bool _isToUp)
{
	if (!Node::init())return false;
	m_pClippNode = ClippingNode::create();
	m_pClippNode->setInverted(false);
	m_pClippNode->setAlphaThreshold(0.5f);
	this->addChild(m_pClippNode);
	m_pStencil = Sprite::create(_stencil);
	m_pClippNode->setStencil(m_pStencil);
	m_pBottom = Sprite::create(_light);
	m_pBottom->setRotation(_angle - 90);
	m_pClippNode->addChild(m_pBottom);
	Vec2 arcHalf = Vec2(0, m_pStencil->getContentSize().width / 2);
	arcHalf.rotate(Vec2(0, 0), _angle);
	Vec2 projectV = arcHalf.project(Vec2(0, m_pStencil->getContentSize().width / 2));
	float A = projectV.length();
	float C = arcHalf.length();
	float B = sqrtf(C*C - A*A);
	Vec2 targetPos;
	if ((A >= 0 && B >= 0) || (A < 0 && B >= 0))
	{
		if (_isToUp)
		{
			targetPos = Vec2(A, B);
			A *= -1;
			B *= -1;
		}
		else
		{
			targetPos = Vec2(-A, -B);
		}
	}
	else if ((A >= 0 && B < 0) || (A < 0 && B < 0))
	{
		if (!_isToUp)
		{
			targetPos = Vec2(A, B);
			A *= -1;
			B *= -1;
		}
		else
		{
			targetPos = Vec2(-A, -B);
		}
	}
	m_pBottom->setPosition(A, B);
	m_pBottom->runAction(Sequence::create(MoveTo::create(_time, targetPos), FadeOut::create(0.1f), CallFunc::create([=](){
		this->removeFromParentAndCleanup(true);
	}), nullptr));
	Global::getInstance()->myNetTime = {};
	return true;
}
//=================================================遮罩可自定义形状的
ClippBlenkCustomShape* ClippBlenkCustomShape::create()
{
	ClippBlenkCustomShape* _cbcs = new ClippBlenkCustomShape();
	if (_cbcs&&_cbcs->init())
	{
		_cbcs->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(_cbcs);
	}
	return _cbcs;
}
bool ClippBlenkCustomShape::init()
{
	if (!Node::init())return false;
	m_bIsClippOver = false;

	LayerColor* _layerColor = LayerColor::create(Color4B(0,0,0,255));

	m_pClippNode = ClippingNode::create();
	m_pClippNode->setInverted(true);
	this->addChild(m_pClippNode);

	m_pClippNode->addChild(_layerColor);

	float _radius = 1.f;
	int _count = 200;
	float _angle = 2.0f*(float)M_PI / _count;
	Vec2 _point[200];
	for (int i = 0; i < _count; i++)
	{
		float _radian = i * _angle;
		_point[i].x = _radius*cosf(_radian);
		_point[i].y = _radius*sinf(_radian);
	}
	m_pStencil = DrawNode::create();
	m_pStencil->drawPolygon(_point, _count, Color4F::RED, 0.01f, Color4F::BLACK);
	m_pClippNode->setStencil(m_pStencil);
	m_pStencil->setPosition(STANDARD_WIDTH / 2.f, 200);
	
	return true;
}
void ClippBlenkCustomShape::runBlenkAction(Vec2 _startPos, float _startScale, float _scaleTarget, float _time, function<void()> _callBack)
{
	m_pStencil->setPosition(_startPos);
	m_pStencil->setScale(_startScale);
	m_pStencil->runAction(Sequence::create(ScaleTo::create(_time, _scaleTarget), CallFunc::create([=](){
		m_bIsClippOver = true;
		if (_callBack)
			_callBack();
	}), nullptr));
}

