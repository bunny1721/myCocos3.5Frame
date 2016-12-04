#pragma once

#include "StructFile.h"

//===========================================精灵模糊
class SpriteBlur : public Sprite
{
public:
	static SpriteBlur* create(string _path ,int _type);
	void setBlurRadius(float _value);
	void setBlurSampleNum(float _value);
	
	
private:
	bool init(string _path, int _type);
	bool initWithTexture(Texture2D *texture, const Rect& rect);
	void initGLPragram();
	void initWithScreenSection(bool _isSaveImage = false);
protected:
	int m_nType;
};

//================================================liquid精灵类
class LiquidSprite : public Node
{
public:
	static LiquidSprite* create(b2ParticleSystem* _b2ParticleSystem , int _ratio);
private:
	bool init(b2ParticleSystem* _b2ParticleSystem, int _ratio);
	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
	virtual void onDraw(const Mat4& transform, uint32_t flags);
	void initLiquidSprite();
protected:
	b2ParticleSystem* m_pB2ParticleSystem;
	float m_fRatio;
	b2Vec2* m_pPosVerts;
	vector<Vec2>m_vecTexCoord;

	CustomCommand m_CCommand;

};

//==============================================精灵变灰
class SpriteGray : public Sprite
{
public:
	static SpriteGray* create(string _path);
	void setGray();
	void releaseGray();
private:
	bool init(string _path);
};