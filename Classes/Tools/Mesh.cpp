/****************************************************************************
Copyright (c) 2011      ForzeField Studios S.L.
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN false EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "Mesh.h"
#include "math/CCVertex.h"
#include "base/ccMacros.h"
#include "base/CCDirector.h"
#include "base/CCConfiguration.h"
#include "renderer/CCTextureCache.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramState.h"
#include "renderer/CCCustomCommand.h"
#include "renderer/CCRenderer.h"

NS_CC_BEGIN

MeshSprite::MeshSprite()
: _texture(nullptr)
, _blendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED)
, _vertices(nullptr)
, _colorPointer(nullptr)
, _texCoords(nullptr)
, _dirty(false)
{
}

MeshSprite::~MeshSprite()
{
	CC_SAFE_RELEASE(_texture);
	CC_SAFE_FREE(_vertices);
	CC_SAFE_FREE(_colorPointer);
	CC_SAFE_FREE(_texCoords);
}

MeshSprite* MeshSprite::create(int rows, int columns, const std::string& path)
{
	MeshSprite *ret = new MeshSprite();
	if (ret && ret->initWithFade(rows, columns, path))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

MeshSprite* MeshSprite::create(int rows, int columns, Texture2D* texture)
{
	MeshSprite *ret = new MeshSprite();
	if (ret && ret->initWithFade(rows, columns, texture))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool MeshSprite::initWithFade(int rows, int columns, const std::string& path)
{
	CCASSERT(!path.empty(), "Invalid filename");

	Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(path);
	return initWithFade(rows, columns, texture);
}

bool MeshSprite::initWithFade(int rows, int columns, Texture2D* texture)
{
	setContentSize(texture->getContentSize());

	//    Node::setPosition(Vec2::ZERO);
	//    setAnchorPoint(Vec2::ZERO);
	//    ignoreAnchorPointForPosition(true);

	_rows = rows;
	_columns = columns;

	_total = _rows * _columns;
	_datas = (V2F_C4B_T2F*)malloc(sizeof(V2F_C4B_T2F)* _total);

	memset(_datas, 0, _total * sizeof(V2F_C4B_T2F));

	for (int i = 0; i<_total; i++)
	{
		_datas[i].colors = Color4B(255, 255, 255, 255);
	}

	// indices
	_indiceNumber = (_rows - 1) * (_columns - 1) * 6;
	_indices = (GLushort *)malloc(_indiceNumber * sizeof(GLushort));

	memset(_indices, 0, _indiceNumber * sizeof(GLushort));

	setupIndices();

	//是否支持顶点数组对象
	if (Configuration::getInstance()->supportsShareableVAO())
	{
		setupVBOandVAO();
	}
	else
	{
		//设置顶点数组缓存对象
		setupVBO();
	}

	// Set blend mode
	_blendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;

	// shader state
	setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));

	setTexture(texture);
	scheduleUpdate();

	return true;
}

void MeshSprite::setupIndices()
{
	if (_total == 0)
		return;

	int i = 0, index = 0;
	for (int y = 0; y < _rows - 1; ++y)
	{
		for (int x = 0; x < _columns - 1; ++x)
		{
			index = x + y * _columns;
			_indices[i] = index;
			_indices[i + 1] = index + _columns;
			_indices[i + 2] = index + 1;

			_indices[i + 3] = index + _columns + 1;
			_indices[i + 4] = index + 1;
			_indices[i + 5] = index + _columns;

			i += 6;
		}
	}
}

void MeshSprite::setupVBOandVAO()
{
	//创建一个顶点数组对象
	glGenVertexArrays(1, &_VAOname);
	//绑定顶点数组对象
	GL::bindVAO(_VAOname);

#define kQuadSize sizeof(_datas[0])

	//创建两个顶点缓存对象
	glGenBuffers(2, &_buffersVBO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, _buffersVBO[0]);
	//设置顶点数组对象的size 设置为动态绘制
	glBufferData(GL_ARRAY_BUFFER, kQuadSize * _total, _datas, GL_DYNAMIC_DRAW);

	// vertices
	//禁用顶点数组对象
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*)offsetof(V2F_C4B_T2F, vertices));

	// colors
	//使用颜色顶点数组对象
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);

	//参数1 ： 要修改的顶点属性
	//参数2 ： 制定每个顶点属性的数量(1 , 2 ,3 或 4 初始为4) 如 左边 （x,y,y）  颜色 （r, g, b , a）
	//参数3 ： 指定顶点属性每个的数据类型 GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT,GL_UNSIGNED_SHORT, GL_FIXED, 和 GL_FLOAT，初始值为GL_FLOAT。
	//参数4 ： normalized被设置为GL_TRUE，意味着整数型的值会被映射至区间[-1,1](有符号整数)，或者区间[0,1]（无符号整数），反之，这些值会被直接转换为浮点值而不进行归一化处理。
	//参数5 ： 指定连续顶点属性之间的偏移量。如果为0，那么顶点属性会被理解为：它们是紧密排列在一起的。初始值为0。
	/*参数6 ： 如果一个名称非零的缓冲对象被绑定至GL_ARRAY_BUFFER目标（见glBindBuffer）且此时一个定点属性数组被指定了，那么pointer被当做该缓冲对象数据存储区的字节偏移量。并且，
			   缓冲对象绑定（GL_ARRAY_BUFFER_BINDING）会被存为索引为index的顶点属性数组客户端状态（GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING）；（Also, the buffer object binding 
			   (GL_ARRAY_BUFFER_BINDING) is saved as generic vertex attribute array client-side state (GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING) for index index.）
	           当一个顶点属性数组被指定时，除了当前的顶点数组缓冲对象绑定，size, type, normalized, stride, 和 pointer 也会被存为客户端状态
		       要启用或者禁用顶点属性数组，调用glEnableVertexAttribArray和glDisableVertexAttribArray传入参数index。如果启用，
			   那么当glDrawArrays或者glDrawElements被调用时，顶点属性数组会被使用。*/
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (GLvoid*)offsetof(V2F_C4B_T2F, colors));

	// tex coords \
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*)offsetof(V2F_C4B_T2F, texCoords));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffersVBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * _indiceNumber, _indices, GL_STATIC_DRAW);

	// Must unbind the VAO before changing the element buffer.  改变顶点数组对象的元素缓存之前必须要未绑定
	GL::bindVAO(0);
	//激活缓冲区对象
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//检查错误
	CHECK_GL_ERROR_DEBUG();
}

void MeshSprite::setupVBO()
{
	glGenBuffers(2, &_buffersVBO[0]);

	mapBuffers();
}

void MeshSprite::mapBuffers()
{
	// Avoid changing the element buffer for whatever VAO might be bound.
	//避免改变元素的缓冲无论vao 是否绑定
	GL::bindVAO(0);

	glBindBuffer(GL_ARRAY_BUFFER, _buffersVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_datas[0]) * _total, _datas, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffersVBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * _indiceNumber, _indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	CHECK_GL_ERROR_DEBUG();
}

void MeshSprite::setVertice(int row, int column, const Point &p)
{
	_datas[row*_columns + column].vertices = Vec2(p.x, p.y);
	_dirty = true;
}

Point MeshSprite::getVertice(int row, int column)
{
	return _datas[row*_columns + column].vertices;
}

void MeshSprite::setTexCoord(int row, int column, const Point &tex)
{
	_datas[row*_columns + column].texCoords = Tex2F(tex.x, tex.y);
	_dirty = true;
}

Point MeshSprite::getTexCoord(int row, int column)
{
	Tex2F& t = _datas[row*_columns + column].texCoords;

	return Point(t.u, t.v);
}

void MeshSprite::tintWithColor(const Color3B& colors)
{
	setColor(colors);

	// Fast assignation
	for (unsigned int i = 0; i<_rows * 2; i++)
	{
		*((Color3B*)(_colorPointer + i * 4)) = colors;
	}
}

Texture2D* MeshSprite::getTexture(void) const
{
	return _texture;
}

void MeshSprite::setTexture(Texture2D *texture)
{
	if (_texture != texture)
	{
		CC_SAFE_RETAIN(texture);
		CC_SAFE_RELEASE(_texture);
		_texture = texture;
	}
}

void MeshSprite::setBlendFunc(const BlendFunc &blendFunc)
{
	_blendFunc = blendFunc;
}

const BlendFunc& MeshSprite::getBlendFunc(void) const
{
	return _blendFunc;
}

void MeshSprite::setOpacity(GLubyte opacity)
{
	CCASSERT(false, "Set opacity no supported");
}

GLubyte MeshSprite::getOpacity(void) const
{
	CCASSERT(false, "Opacity no supported");
	return 0;
}

void MeshSprite::setOpacityModifyRGB(bool bValue)
{
	CC_UNUSED_PARAM(bValue);
}

bool MeshSprite::isOpacityModifyRGB(void) const
{
	return false;
}

void MeshSprite::update(float delta)
{

}

void MeshSprite::reset()
{
	_rows = 0;
}

void MeshSprite::onDraw(const Mat4 &transform, uint32_t flags)
{
	getGLProgram()->use();
	getGLProgram()->setUniformsForBuiltins(transform);

	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
	GL::blendFunc(_blendFunc.src, _blendFunc.dst);

	GL::bindTexture2D(_texture->getName());
	if (Configuration::getInstance()->supportsShareableVAO())
	{
		//
		// Using VBO and VAO
		//

		// XXX: update is done in draw... perhaps it should be done in a timer
		if (_dirty)
		{
			glBindBuffer(GL_ARRAY_BUFFER, _buffersVBO[0]);
			// option 1: subdata
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(_datas[0]) * _total, _datas);

			// option 2: data
			//            glBufferData(GL_ARRAY_BUFFER, sizeof(quads_[0]) * (n-start), &quads_[start], GL_DYNAMIC_DRAW);

			// option 3: orphaning + glMapBuffer
			//            glBufferData(GL_ARRAY_BUFFER, sizeof(_datas[0]) * _total, nullptr, GL_DYNAMIC_DRAW);
			//            void *buf = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			//            memcpy(buf, _datas, sizeof(_datas[0])* _total);
			//            glUnmapBuffer(GL_ARRAY_BUFFER);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			_dirty = false;
		}

		GL::bindVAO(_VAOname);

#if CC_REBIND_INDICES_BUFFER
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffersVBO[1]);
#endif

		glDrawElements(GL_TRIANGLES, (GLsizei)_indiceNumber, GL_UNSIGNED_SHORT, (GLvoid*)0);

#if CC_REBIND_INDICES_BUFFER
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif

		//    glBindVertexArray(0);
	}
	else
	{
#define kQuadSize sizeof(_datas[0])


		glBindBuffer(GL_ARRAY_BUFFER, _buffersVBO[0]);


		if (_dirty)
		{
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(_datas[0]) * _total, _datas);
			_dirty = false;
		}

		GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);

		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*)offsetof(V2F_C4B_T2F, vertices));
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*)offsetof(V2F_C4B_T2F, texCoords));
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (GLvoid*)offsetof(V2F_C4B_T2F, colors));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffersVBO[1]);

		glDrawElements(GL_TRIANGLES, (GLsizei)_indiceNumber, GL_UNSIGNED_SHORT, (GLvoid*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _total);
}

void MeshSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	if (_total == 0)
		return;
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(MeshSprite::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

NS_CC_END