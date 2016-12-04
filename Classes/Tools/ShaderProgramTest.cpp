#include "ShaderProgramTest.h"

//===========================================精灵模糊
SpriteBlur* SpriteBlur::create(string _path ,int _type)
{
	SpriteBlur* _sb = new SpriteBlur();
	if (_sb && _sb->init(_path , _type))
	{
		_sb->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(_sb);
	}
	return _sb;
}
bool SpriteBlur::init(string _path, int _type)
{
	m_nType = _type;
	if (_type == BlurModeType::myBlurModeNormal)
	{
		this->initWithFile(_path);
		return true;
	}
	else if (_type == BlurModeType::myBlurModeScreenSection)
	{
		this->initWithScreenSection();
		return true;
	}
	return false;
}
bool SpriteBlur::initWithTexture(Texture2D *texture, const Rect& rect)
{
	if (Sprite::initWithTexture(texture, rect))
	{
//#if (CC_ENABLE_CACHE_TEXTURE_DATA)
//		auto listener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom* event){
//			setGLProgram(nullptr);
//			this->initGLProgram();
//		});
//
//		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//#endif

		this->initGLPragram();
		return true;
	}
	return false;
}
void SpriteBlur::initWithScreenSection(bool _isSaveImage)
{
	RenderTexture* _renderTexture = RenderTexture::create(Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
	_renderTexture->begin();
	Director::getInstance()->getRunningScene()->visit();
	_renderTexture->end();
	if (_isSaveImage)
	{
		_renderTexture->saveToFile("screenShot.png" ,Image::Format::JPG);
	}
	Texture2D* _texture = _renderTexture->getSprite()->getTexture();
	if (_texture)
	{
		Rect rect = Rect::ZERO;
		rect.size = _texture->getContentSize();
		this->initWithTexture(_texture, rect);
	}
	else
		CCASSERT(0 , "get texture failed!");
}
void SpriteBlur::initGLPragram()
{
	GLchar* _glSource = (GLchar*)String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename("shader/spriteBlur.fsh").c_str())->getCString();
	auto _program = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, _glSource);
	auto _programState = GLProgramState::getOrCreateWithGLProgram(_program);
	setGLProgramState(_programState);
	auto _size = getTexture()->getContentSizeInPixels();
	getGLProgramState()->setUniformVec2("resolution" , _size);
	getGLProgramState()->setUniformFloat("blurRadius" , 0.f);
	getGLProgramState()->setUniformFloat("sampleNum", 0.f);
}
void SpriteBlur::setBlurRadius(float _value)
{
	getGLProgramState()->setUniformFloat("blurRadius", _value);
}
void SpriteBlur::setBlurSampleNum(float _value)
{
	getGLProgramState()->setUniformFloat("sampleNum", _value);
}

//================================================liquid精灵类

LiquidSprite* LiquidSprite::create(b2ParticleSystem* _b2ParticleSystem, int _ratio)
{
	LiquidSprite* _ls = new LiquidSprite();
	if (_ls && _ls->init(_b2ParticleSystem, _ratio))
	{
		_ls->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(_ls);
	}
	return _ls;
}
bool LiquidSprite::init(b2ParticleSystem* _b2ParticleSystem, int _ratio)
{
	if (!Node::init())return false;

	m_pB2ParticleSystem = _b2ParticleSystem;
	m_fRatio = _ratio;

	this->initLiquidSprite();
	return true;
}

void LiquidSprite::initLiquidSprite()
{
	auto _uTexture = Director::getInstance()->getTextureCache()->addImage("water.png");
	auto _texSize = _uTexture->getContentSize();
	auto _program = GLProgram::createWithFilenames("shader/liquid.vert", "shader/liquid.frag");
	auto _state = GLProgramState::getOrCreateWithGLProgram(_program);

	_state->setUniformTexture("u_texture", _uTexture);
	_state->setUniformFloat("u_ratio", m_fRatio);
	_state->setUniformVec4("u_color", Vec4(1, 1, 1, 0.7));
	_state->setUniformFloat("u_pointSize", m_pB2ParticleSystem->GetRadius()*m_fRatio * 2);
	this->setGLProgramState(_state);

	auto _vertsCount = m_pB2ParticleSystem->GetParticleCount();
	m_pPosVerts = m_pB2ParticleSystem->GetPositionBuffer();

	for (int i = 0; i < _vertsCount; i++)
	{
		b2Vec2 _vec = m_pPosVerts[i];
		m_vecTexCoord.push_back(Vec2(_vec.x*m_fRatio / _texSize.width, 1 - (_vec.y*m_fRatio) / _texSize.height));
	}
	_state->setVertexAttribPointer("a_position", 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), &m_pPosVerts[0]);
	_state->setVertexAttribPointer("a_texCoord", 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), &m_vecTexCoord[0]);
}
void LiquidSprite::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	m_CCommand.init(_globalZOrder, transform, flags);
	m_CCommand.func = CC_CALLBACK_0(LiquidSprite::onDraw, this, transform, flags);
	renderer->addCommand(&m_CCommand);
}
void LiquidSprite::onDraw(const Mat4& transform, uint32_t flags)
{
	this->getGLProgramState()->apply(transform);
	glDrawArrays(GL_POINTS , 0 ,m_pB2ParticleSystem->GetParticleCount());
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1 , m_pB2ParticleSystem->GetParticleCount());
}
//==============================================精灵变灰
SpriteGray* SpriteGray::create(string _path)
{
	SpriteGray* _sg = new SpriteGray();
	if (_sg && _sg->init(_path))
	{
		_sg->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(_sg);
	}
	return _sg;
}
bool SpriteGray::init(string _path)
{
	if (!this->initWithFile(_path))return false;

	return true;
}

void SpriteGray::setGray()
{
	GLProgram* _p = new GLProgram();
	_p->initWithFilenames("shader/gray.vsh", "shader/gray.fsh");
	_p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
	_p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
	_p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
	_p->link();
	_p->updateUniforms();
	this->setGLProgram(_p);
}
void SpriteGray::releaseGray()
{
	string str = "ShaderPositionTextureColor_noMVP";
	GLProgram* _pProgram = ShaderCache::getInstance()->getGLProgram(str);
	this->setGLProgram(_pProgram);
	CHECK_GL_ERROR_DEBUG();
}