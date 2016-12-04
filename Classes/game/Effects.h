#pragma once
#include "Tools\CommonFile.h"

//===================================================∆’Õ®Ãÿ–ß÷°
class NormalEffectFrame : public Node
{
public:
	typedef function<void()> FUNCCUSTOM;
	static NormalEffectFrame* create(string _path, int _length, int _loopCount, Vec2 _startPos, FUNCCUSTOM m_funcCustom = nullptr);
private:
	bool init(string _path, int _length, int _loopCount,  Vec2 _startPos, FUNCCUSTOM m_funcCustom);
protected:
	Sprite* m_pSprite;
	int m_nLength;
	FUNCCUSTOM m_funcCustom;
};