#include "Effects.h"



//===================================================ÆÕÍ¨ÌØÐ§Ö¡
NormalEffectFrame* NormalEffectFrame::create(string _path, int _length, int _loopCount, Vec2 _startPos, FUNCCUSTOM m_funcCustom)
{
	NormalEffectFrame* _ss = new NormalEffectFrame();
	if (_ss && _ss->init(_path, _length, _loopCount, _startPos, m_funcCustom))
	{
		_ss->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(_ss);
	}
	return _ss;
}
bool NormalEffectFrame::init(string _path, int _length, int _loopCount, Vec2 _startPos, FUNCCUSTOM m_funcCustom)
{
	if (!Node::init())return false;

	m_nLength = _length;
	this->setPosition(_startPos);
	m_pSprite = Sprite::create(_path);
	this->addChild(m_pSprite);
	ActionInterval* _frameAct;
	myGameInstance->createActionForFrameSprite(_frameAct, _path, _length, _loopCount);
	m_pSprite->runAction(Sequence::create(_frameAct, CallFunc::create([=](){
		if (m_nLength != -1)
		{
			if (m_funcCustom)
			{
				m_funcCustom();
			}
			this->removeFromParentAndCleanup(true);
		}
	}), nullptr));
	return true;
}
